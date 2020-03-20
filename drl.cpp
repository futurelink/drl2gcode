#include "drl.h"

using namespace std;

DRLFile::DRLFile(std::string name) {
    this->name = name;
    regex_tool_def = new std::regex("T(\\d+)(C(\\d+.\\d+))?\\n?");
    regex_measure = new std::regex("(METRIC|INCH),(\\d+)\\.(\\d+)\\n?");
    regex_point = new std::regex("[Xx]([\\+-]?\\d+)[Yy]([\\+-]?\\d+)\\n?");
}

/**
 * Add a tool to tool table
 */
void DRLFile::add_tool(unsigned int tool_number, float tool_dia) {
    DRLTool *tool = new DRLTool;
    tool->number = tool_number;
    tool->diameter = tool_dia;
    info.add_tool(tool);
}

/**
 * Add block to make with specified tool
 */
void DRLFile::add_block(DRLTool *tool) {
    current_block = new DRLBlock(tool);
    blocks.push_back(current_block);
}

/**
 * Get a tool from tool table
 */
DRLTool *DRLFile::get_tool(unsigned int tool_number) {
    return info.get_tool(tool_number);
}

/**
 * Open a DRL file for reading
 */
bool DRLFile::open() {
    this->fd = fopen(this->name.data(), "r");
    return this->fd != NULL;
}

/**
 * Parse a DRL file
 */
void DRLFile::parse() {
    char buffer[BUFFER_LEN];
    char log_str[255];
    cmatch match;
    bool header_section = true;
    while (fgets(buffer, BUFFER_LEN, this->fd)) {
	if (regex_match(buffer, match, *regex_tool_def)) {
	    unsigned int tool_number = atoi(match[1].str().data());
	    float tool_dia = atof(match[3].str().data());
	    if (header_section) {
		add_tool(tool_number, tool_dia);
		sprintf(log_str, "tool definition %d of dia %f", tool_number, tool_dia);
	    } else {
		if (tool_number == 0) {
		    log("unload tool");
		} else {
		    DRLTool *tool = get_tool(tool_number);
		    if (tool != nullptr) {
			add_block(tool);
			sprintf(log_str, "began block with tool number %d", tool_number);
		    } else {
			error("unknown tool");
		    }
		}
	    }
	    log(log_str);
	} else if (regex_match(buffer, match, *regex_measure)) {
	    string measure = match[1].str();
	    if (measure.compare("METRIC") == 0) {
		info.measure = METRIC;
	    } else if (measure.compare("INCH") == 0) {
		info.measure = INCH;
	    }
	    info.decimal_count = match[2].str().length();
	    info.fraction_count = match[3].str().length();

	    sprintf(log_str, "decimals %d, fraction %d", info.decimal_count, info.fraction_count);
	    log(log_str);
	} else if (regex_match(buffer, match, *regex_point)) {
	    float x, y;
	    if (parse_point_coord(match[1].str(), &x) && parse_point_coord(match[2].str(), &y)) {
		current_block->add_point(x, y);
		sprintf(log_str, "point coords X=%f, Y=%f", x, y);
		log(log_str);
	    } else {
		error("invalid point value");
	    }
	} else if (buffer[0] == '%') {
	    log("data section began");
	    header_section = false;
	} else {
	    sprintf(log_str, "read unparsed: %s", buffer);
	    log(log_str);
	}
    }
}

/**
 * Parse coordinate with specified decimal and fraction places
 * read from header of DRL file.
 */
bool DRLFile::parse_point_coord(string str, float *value) {
    string decimal, fraction;
    bool has_sign = ((str.at(0) == '+') || (str.at(0) == '-'));
    if (str.length() != (info.decimal_count + info.fraction_count + (has_sign ? 1 : 0)))	// +1 for sign place
	return false;

    decimal = str.substr(0, info.decimal_count + (has_sign ? 1 : 0));
    fraction = str.substr(info.decimal_count + (has_sign ? 1 : 0), info.fraction_count);
    *value = atoi(decimal.data()) + atoi(fraction.data()) / pow(10, info.fraction_count);

    return true;
}

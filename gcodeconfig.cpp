#include "gcodeconfig.h"
#include <regex>

bool GCodeConfig::load(std::string file_name) {
    std::FILE *fd = fopen(file_name.data(), "r");
    if (!fd) {
	return false;
    }

    char buf[1024];
    std::regex section_regex("\\[(.*?)\\]\n?");
    std::regex param_regex("(\\w+) *= *(.*) *\n?");
    std::cmatch match;

    while(fgets(buf, 1024, fd)) {
	if (regex_match(buf, match, section_regex)) {
	    // Sections are not needed for now
	} else if (regex_match(buf, match, param_regex)) {
	    std::string name = match[1].str();
	    std::string value = match[2].str();
	    if (name.compare("init_line") == 0) {
		this->init_line = value;
	    } else if (name.compare("safe_z") == 0) {
		this->safe_z = atof(value.data());
	    } else if (name.compare("spindle_on_cmd") == 0) {
		this->spindle_on_cmd = value;
	    } else if (name.compare("spindle_off_cmd") == 0) { 
		this->spindle_off_cmd = value;
	    } else if (name.compare("spindle_speed") == 0) { 
		this->spindle_speed = atoi(value.data());
	    } else if (name.compare("tool_change_z") == 0) { 
		this->tool_change_z = atof(value.data());
	    } else if (name.compare("move_feed") == 0) { 
		this->move_feed = atoi(value.data());
	    } else if (name.compare("drill_up_feed") == 0) { 
		this->drill_up_feed = atof(value.data());
	    } else if (name.compare("drill_down_feed") == 0) { 
		this->drill_down_feed = atof(value.data());
	    } else if (name.compare("drill_down_z") == 0) { 
		this->drill_down_z = atof(value.data());
	    } else {
		printf("%s = %s\n", name.data(), value.data());
	    }
	}
    }
    fclose(fd);

    return true;
}

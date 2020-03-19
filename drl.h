#include <vector>
#include <string>
#include <cstdio>
#include <regex>
#include <math.h>

#ifndef _DRL_H_
#define _DRL_H_

#define BUFFER_LEN 1024

enum Measure { METRIC, INCH };

typedef struct DRLTool {
    unsigned int number;
    float diameter;
} DRLTool;

typedef struct DRLPoint {
    float x;
    float y;
} DRLPoint;

class DRLInfo {
    // Tool table
    std::vector<DRLTool *> tools;

public:
    Measure measure;
    int decimal_count;
    int fraction_count;

    DRLInfo() {}
    ~DRLInfo() {
	// Clear tool table
    }

    DRLTool *get_tool(unsigned int tool_number) {
	for(auto const& tool: tools) {
	    if (tool->number == tool_number) return tool;
	}
	return nullptr;
    }

    void add_tool(DRLTool *tool) {
	tools.push_back(tool);
    }
};

class DRLBlock {
    DRLTool *tool;
    std::vector<DRLPoint> points;

public:
    DRLBlock(DRLTool *tool) {
	this->tool = tool;
    }

    ~DRLBlock() {}

    void add_point(float x, float y) {
	DRLPoint point;
	point.x = x;
	point.y = y;
	points.push_back(point);
    }
};

class DRLFile {
    DRLInfo info;
    std::vector<DRLBlock *> blocks;
    DRLBlock *current_block;

    std::string name;
    std::FILE *fd;

    std::regex *regex_measure;
    std::regex *regex_tool_def;
    std::regex *regex_point;

public:
    DRLFile(std::string name);

    bool open();
    void parse();

    void error(const char *msg) { printf("%s\n", msg); }
    void log(const char *msg) { printf("%s\n", msg); }

private:
    void add_tool(unsigned int tool_number, float tool_dia);
    void add_block(DRLTool *tool);
    DRLTool *get_tool(unsigned int tool_number);
    bool parse_point_coord(std::string str, float *value);
};

#endif

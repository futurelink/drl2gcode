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

    DRLTool *get_tool(unsigned int tool_number);

    void add_tool(DRLTool *tool);
};

class DRLBlock {
    DRLTool *tool;
    std::vector<DRLPoint> points;

public:
    DRLBlock(DRLTool *tool);

    ~DRLBlock() {}

    unsigned int tool_number();

    void add_point(float x, float y);

    size_t points_count();
    DRLPoint point(size_t num);
};

class DRLFile {
    std::vector<DRLBlock *> blocks;
    DRLBlock *current_block;

    std::string name;
    std::FILE *fd;

    std::regex *regex_measure;
    std::regex *regex_tool_def;
    std::regex *regex_point;

public:
    DRLInfo info;

    DRLFile(std::string name);

    bool open();
    void parse();

    size_t blocks_count();
    DRLBlock *block(size_t num);

    void error(const char *msg);
    void log(const char *msg);

private:
    void add_tool(unsigned int tool_number, float tool_dia);
    void add_block(DRLTool *tool);
    DRLTool *get_tool(unsigned int tool_number);
    bool parse_point_coord(std::string str, float *value);
};

#endif

#include <string>

#include "drl.h"

#ifndef _GCODE_H_
#define _GCODE_H_

#define STR_BUF_LEN 1024

typedef struct GCodeConfig {
    std::string init_line;

    float tool_change_z;
    float safe_z;
    float move_feed;

    float drill_up_feed;
    float drill_down_feed;
    float drill_down_z;

    std::string spindle_on_cmd;
    std::string spindle_off_cmd;
    int spindle_speed;
} GCodeConfig;

class GCodeWriter {
    GCodeConfig *config;
    FILE *file;

public:
    GCodeWriter(GCodeConfig *config);

    bool write(DRLFile *file, std::string file_name);

private:
    void write_program_header(DRLFile *drl);
    void write_block_header(DRLBlock *block);
    void write_move(DRLPoint point);
    void write_block_footer(DRLBlock *block);
    void write_program_footer(DRLFile *drl);
};

#endif

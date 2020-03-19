#include <string>

#ifndef _GCODE_H_
#define _GCODE_H_

typedef struct GCodeConfig {
    float safe_z;
    float move_feed;

    float drill_up_feed;
    float drill_down_feed;
    float drill_down_z;
} GCodeConfig;

class GCodeWriter {
    GCodeConfig *config;

public:
    GCodeWriter(GCodeConfig *config);

    void write(std::string file_name);
};

#endif

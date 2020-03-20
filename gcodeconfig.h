#ifndef _GCODECONFIG_H_
#define _GCODECONFIG_H_

#include <string>

class GCodeConfig {
public:
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

    // Load INI file
    bool load(std::string file_name);
};

#endif

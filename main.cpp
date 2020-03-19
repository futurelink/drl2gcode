#include "drl.h"
#include "gcode.h"

int main(int argc, char *argv[]) {
    DRLFile *drl_file = new DRLFile(std::string("example.drl"));
    if (!drl_file->open()) {
	drl_file->error("File open error!");
    }

    drl_file->parse();

    /* Create config and write G-code */
    GCodeConfig *config = new GCodeConfig;
    config->init_line = "G90 G17 G54\n";
    config->safe_z = 5.0;		// 5mm above
    config->move_feed = 500;		// 500mm/min XY feed
    config->drill_up_feed = 200;	// 200mm/min Z up feed
    config->drill_down_feed = 50;	// 50mm/min Z down feed
    config->drill_down_z = 3;		// drill on 3mm below

    GCodeWriter *writer = new GCodeWriter(config);
    writer->write(drl_file, std::string("example.ngc"));

    return 0;
}

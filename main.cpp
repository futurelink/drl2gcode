#include "drl.h"
#include "gcode.h"

int main(int argc, char *argv[]) {
    DRLFile *file = new DRLFile(std::string("example.drl"));
    if (!file->open()) {
	file->error("File open error!");
    }

    file->parse();

    GCodeConfig *config = new GCodeConfig;
    config->safe_z = 5.0;		// 5mm above
    config->move_feed = 500;		// 500mm/min XY feed
    config->drill_up_feed = 200;	// 200mm/min Z up feed
    config->drill_down_feed = 50;	// 50mm/min Z down feed
    config->drill_down_z = 3;		// drill on 3mm below

    GCodeWriter *writer = new GCodeWriter(config);
    writer->write(std::string("example.ngc"));

    return 0;
}

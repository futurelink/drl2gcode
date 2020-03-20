#include "drl.h"
#include "gcode.h"

void print_usage(char *prog_name);

int main(int argc, char *argv[]) {
    if (argc < 2) {
	print_usage(argv[0]);
	return -1;
    }

    DRLFile *drl_file = new DRLFile(std::string(argv[1]));
    if (!drl_file->open()) {
	drl_file->error("File open error!");
    }

    drl_file->parse();

    /* Create config and write G-code */
    GCodeConfig *config = new GCodeConfig();

    // Load config or use default parameters
    if (!config->load("drl2gcode.ini")) {
	printf("No config file, using defaults!");

	config->init_line = "G90 G17 G54\n";
	config->spindle_on_cmd = "M3";
	config->spindle_off_cmd = "M5";
	config->spindle_speed = 10000;
	config->tool_change_z = 40.0;	// 40mm above to release a drill bit
	config->safe_z = 5.0;		// 5mm above
	config->move_feed = 500;		// 500mm/min XY feed
	config->drill_up_feed = 200;	// 200mm/min Z up feed
	config->drill_down_feed = 50;	// 50mm/min Z down feed
	config->drill_down_z = -3;		// drill on 3mm below
    }

    GCodeWriter *writer = new GCodeWriter(config);
    writer->write(drl_file, std::string("output.ngc"));

    return 0;
}

void print_usage(char *prog_name) {
    printf("Usage:\n%s [filename.drl]\n", prog_name);
}
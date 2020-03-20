#include "gcode.h"

GCodeWriter::GCodeWriter(GCodeConfig *config) {
    this->config = config;
}

bool GCodeWriter::write(DRLFile *drl, std::string file_name) {
    this->file = fopen(file_name.data(), "w");
    if (!this->file) return false;

    write_program_header(drl);

    for (auto i = 0; i < drl->blocks_count(); i++) {
	auto block = drl->block(i);
	write_block_header(block);

        // Drill points
        for (auto j = 0; j < block->points_count(); j++) {
	    DRLPoint p = block->point(j);
	    write_move(p);
        }

	write_block_footer(block);
    }

    write_program_footer(drl);

    return true;
}

void GCodeWriter::write_program_header(DRLFile *drl) {
    fputs(config->init_line.data(), this->file);
    if (drl->info.measure == INCH) {
        fputs("G20", this->file);
    } else {
        fputs("G21", this->file);
    }
    fputs("\n", this->file);

    // Go to safe Z on G0
    std::stringstream prepare_cmd;
    prepare_cmd << "G0 Z" << std::to_string(config->safe_z) << " F" << std::to_string((int)config->drill_up_feed)  << "\n";
//    prepare_cmd << config->spindle_on_cmd << " S" << config->spindle_speed << "\n";
    fputs(prepare_cmd.str().data(), this->file);
}

void GCodeWriter::write_block_header(DRLBlock *block) {
    // Setting a tool
    std::stringstream tool_set;
    tool_set
	<< "( --------------------- )\n"
	<< "( Drilling with tool #" << block->tool_number() << " )\n"
	<< "( --------------------- )\n"
	<< config->spindle_off_cmd << "\n"
	<< "G0 Z" << std::to_string(config->tool_change_z) << " F" << std::to_string((int)config->drill_up_feed)  << "\n" // Go to tool change Z
	<< "T" << std::to_string(block->tool_number()) << "M6\n" // Tool set
	<< config->spindle_on_cmd << " S" << config->spindle_speed << "\n";
    fputs(tool_set.str().data(), this->file);

}

void GCodeWriter::write_move(DRLPoint point) {
    // Move to position
    std::stringstream move_cmd;
    move_cmd << "G0 X" << std::to_string(point.x)
	<< " Y" << std::to_string(point.y)
	<< " F" << std::to_string((int)config->move_feed)
	<< "\n";
    fputs(move_cmd.str().data(), this->file);

    // Drill down and raise up
    std::stringstream drill_down_cmd;
    drill_down_cmd << "G1 Z" << std::to_string(config->drill_down_z) << " F" << std::to_string((int)config->drill_down_feed)  << "\n"
	<< "Z" << std::to_string(config->safe_z) << " F" << std::to_string((int)config->drill_up_feed)  << "\n";
    fputs(drill_down_cmd.str().data(), this->file);
}

void GCodeWriter::write_block_footer(DRLBlock *block) {

}

void GCodeWriter::write_program_footer(DRLFile *drl) {
    std::stringstream finish_cmd;
    finish_cmd
	<< config->spindle_off_cmd << "\n" // Turn off spindle
	<< "G0 Z" << std::to_string(config->tool_change_z) << " F" << std::to_string((int)config->drill_up_feed) << "\n"  // Go to tool change Z
	<< "G0 X0 Y0\n"
	<< "\n";
    fputs(finish_cmd.str().data(), this->file);
}

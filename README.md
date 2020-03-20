minimalistic Excellon DRL to GCode converter
============================================

This tool is used to convert PCB drilling file to G-Code to execute it on CNC milling
machines.

I tried to write a minimalistic G-Code which could be undelstood by almost any CNC machine,
i've tested it with LinuxCNC and MACH3.

Usage
---

It has a simple configuration file drl2gcode.ini containing some parameters of your mill such as feed, safe Z, 
spindle speed, spindle commands etc.

Adapt it to your machine and run <i>./drl2gcode [your_drl_file]</i>. The result will be in <i>output.ngc</i> file.


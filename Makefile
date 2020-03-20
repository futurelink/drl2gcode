all:
	gcc -std=c++11 -o drl2gcode main.cpp drl.cpp gcode.cpp gcodeconfig.cpp -lstdc++ -lm

#include <iostream>

#include <JLynCore.h>
#include <SFML.h>

int main(int* argc, char** argv) {
	jlyn::Program* sandbox = new jlyn::Program(1280, 720, "JLyn", false);
	sandbox->Run();

	delete sandbox;
	return 0;
}
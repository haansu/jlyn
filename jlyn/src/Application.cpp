#include <iostream>

#include <JLynCore.h>

// Entry Point of the Program
int main(int* argc, char** argv) {
	jlyn::Log::Init();
	jlyn::Program* sandbox = new jlyn::Program(1280, 720, "JLyn", true);
	sandbox->Run();
	delete sandbox;
	return 0;
}
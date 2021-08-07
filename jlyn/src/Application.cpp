#include <JLyn.h>


// Entry Point of the Program
int main(int argc, char** argv) {

	#ifdef JLYN_RELEASE or JLYN_DIST
		ShowWindow(GetConsoleWindow(), SW_HIDE);
	#endif

	jlyn::Log::Init();

	for (int i = 0; i < argc; i++)
		CORE_INFO("Arguement-{0}: {1}", i, argv[i]);

	// Checks if application has been run with a filepath arguement and gives default arguement otherwise
	char* argFile;
	if (argc < 2)
		argFile = "[]none";
	else
		argFile = argv[1];

	jlyn::Program* sandbox = new jlyn::Program(1280, 720, "JLyn", true, argv[0], argFile);
	sandbox->Run();
	delete sandbox;

	return 0;
}
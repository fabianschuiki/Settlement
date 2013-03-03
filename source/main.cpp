/* Copyright © 2013 Fabian Schuiki */
#include <gc_cpp.h>
#include "Application.h"

/**
 * Main function of the game. Initializes a new instance of Application and
 * calls Application::run() on it.
 */
int main(int argc, char *argv[])
{
	// Setup the garbage collector.
	GC_INIT();

	// Initialize the application and run.
	Application *app = new Application();
	return app->run();
}
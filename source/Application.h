/* Copyright © 2013 Fabian Schuiki */
#pragma once
#include "Logger.h"
#include "Scene.h"
#include "RenderInfo.h"
#include "Console.h"
#include "ImageAsset.h"
#include "StringAsset.h"
#include <gc_cpp.h>
#include <SFML/Window.hpp>
#include <string>
#include <vector>

/**
 * First class to be instantiated. Manages the entire application run cycle.
 * All the magic happens inside the run() function.
 */

class Application : public gc, public ConsoleCommandProvider
{
public:
	Logger logger;

	Application();
	int run();

	ConsoleCommandGroup getConsoleCommands();

	ImageAssetManager imageAssets;
	StringAssetManager stringAssets;

private:
	sf::Window window;
	Scene *scene;

	void initialize();
	void mainLoop();
	void cleanUp();

	bool handleEvent(const sf::Event &event);

	RenderInfo info;

	// Command line interface.
	ConsoleCommandGroup cli;
	void cli_help(ConsoleCall&);
	void cli_quit(ConsoleCall&);

protected:
	friend class ConsoleWindow;
	void executeConsoleCommand(const std::string& input, const std::vector<std::string>& args);
};
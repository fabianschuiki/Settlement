/* Copyright © 2013 Fabian Schuiki */
#pragma once
#include "Logger.h"
#include "Scene.h"
#include "RenderInfo.h"
#include <gc_cpp.h>
#include <SFML/Window.hpp>
#include <string>
#include <vector>

/**
 * First class to be instantiated. Manages the entire application run cycle.
 * All the magic happens inside the run() function.
 */

class Application : public gc
{
public:
	Logger logger;

	Application();
	int run();

	void executeConsoleCommand(std::vector<std::string> args);

private:
	sf::Window window;
	Scene *scene;

	void initialize();
	void mainLoop();
	void cleanUp();

	bool handleEvent(const sf::Event &event);

	RenderInfo info;
};
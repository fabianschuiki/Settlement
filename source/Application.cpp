/* Copyright © 2013 Fabian Schuiki */
#include "Application.h"
#include "GameScene.h"
#include <SFML/OpenGl.hpp>

// Enable the use of the logger's LOG macro.
#define app this

Application::Application()
{
	scene = NULL;
}

/**
 * Contains the entire life cycle of the application.
 */
int Application::run()
{
	logger.push();

	// TODO: Add catch block here.
	initialize();
	mainLoop();
	cleanUp();

	// If we've come this far, nothing bad has happened.
	logger.pop();
	return 0;
}

/**
 * Initializes all components that are required for running the game.
 */
void Application::initialize()
{
	// Make the logger verbose in debug builds.
	#ifdef BUILD_DEBUG
	logger.setLevel(Logger::kLogDebug);
	#endif

	// Indicate the stage of the game.
	LOG(kLogImportant, "Initializing Game");

	// Initialize the window.
	LOG(kLogDebug, "Creating Window");
	sf::ContextSettings settings;
	settings.depthBits = 24;
	window.create(sf::VideoMode(1280, 768), "Settlement", sf::Style::Default, settings);
	window.setVerticalSyncEnabled(true);

	// Setup the OpenGL state.
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClearColor(0, 0, 0, 0);

	// Initialize the game scene. Will change in the future.
	GameScene *gs = new GameScene(this);
	gs->initialize();
	scene = gs;
}

/**
 * Runs the game by entering the main run loop, processing events and handling
 * rendering of the window.
 */
void Application::mainLoop()
{
	// Indicate the stage of the game.
	LOG(kLogImportant, "Entering Main Loop");

	// Main Loop.
	sf::Clock clock;
	while (window.isOpen())
	{
		// Poll events.
		sf::Event event;
		while (window.pollEvent(event)) {
			if (scene && scene->handleEvent(event)) continue;
			if (handleEvent(event)) continue;
		}

		if (!window.isOpen())
			break;

		// Measure the time.
		double dtr = clock.getElapsedTime().asSeconds();
		clock.restart();
		double dt = std::min(dtr, 1.0/20);

		// Advance the simulation.
		if (scene) {
			scene->advance(dt);
		}

		// Draw the scene.
		if (scene) {
			RenderInfo info;
			info.width  = window.getSize().x;
			info.height = window.getSize().y;
			scene->draw(info);
		}

		// Dump any OpenGL errors.
		GLenum err = glGetError();
		if (err != GL_NO_ERROR) {
			LOG(kLogError, "OpenGL error: 0x%x (%i)", (int)err, (int)err);
		}

		// Swap the display buffers.
		window.display();
	}
}

/**
 * Called after the mainLoop() terminates, in order to properly shut down all
 * the subsystems.
 */
void Application::cleanUp()
{
	// Indicate the stage of the game.
	LOG(kLogImportant, "Shutting Down Game");
}

/**
 * Handles SFML events generated by the window.
 */
bool Application::handleEvent(const sf::Event &event)
{
	if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
		window.close();
		return true;
	}
	if (event.type == sf::Event::Resized) {
		LOG(kLogWarning, "Window resizing is not being handled!");
		return true;
	}
	return false;
}
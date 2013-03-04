/* Copyright © 2013 Fabian Schuiki */
#pragma once
#include "ApplicationObject.h"
#include "RenderInfo.h"
#include <SFML/Window/Event.hpp>
#include <gc_cpp.h>

/**
 * Superclass of all objects that may draw graphics to the screen. Different
 * game states such as menus and the game itself derive from this class and
 * implement the draw() and advance() function.
 */

class Scene : public gc, public ApplicationObject
{
public:
	Scene(Application *app) : ApplicationObject(app) {}

	/**
	 * @brief Handles a window event.
	 * Called whenever an event needs to be processed. Return true if the event
	 * was handled, or false if some other portion of the game needs to process
	 * it.
	 */
	virtual bool handleEvent(const sf::Event &event) = 0;

	/**
	 * @brief Advances the simulation.
	 * Perform all the simulation calculation in this function. This might
	 * later be migrated to its own thread or threads. At the moment, expect
	 * this function to be called on the main thread.
	 */
	virtual void advance(double dt) = 0;

	/**
	 * @brief Draws the scene.
	 * The info object contains information about the target that is being
	 * rendered to, such as width, height, depth, etc.. The scene object should
	 * expect the draw function to be called multiple times per frame due to
	 * multiple render passes.
	 */
	virtual void draw(const RenderInfo &info) = 0;
};
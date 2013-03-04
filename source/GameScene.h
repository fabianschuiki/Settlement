/* Copyright © 2013 Fabian Schuiki */
#pragma once
#include "Scene.h"
#include "OrbitingCamera.h"

/**
 * @brief Renders regular gameplay.
 */

class GameScene : public Scene
{
public:
	GameScene(Application *app);

	void initialize();


	bool handleEvent(const sf::Event &event);
	void advance(double dt);
	void draw(const RenderInfo &info);

	OrbitingCamera camera;
};
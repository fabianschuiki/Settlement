/* Copyright © 2013 Fabian Schuiki */
#pragma once
#include "Scene.h"
#include "OrbitingCamera.h"
#include "Simulation.h"

class Terrain;
class TerrainRenderer;
class ConsoleWindow;
class WorldTerrain;

namespace ui {
	class Manager;
}

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
	ui::Manager* ui;

protected:
	Terrain* terrain;
	TerrainRenderer* terrainRenderer;
	bool wireframe;
	int mouse_x0, mouse_y0;
	bool mouseDraggingRight;
	Line clickRay;
	Simulation simulation;
	ConsoleWindow* console;
	WorldTerrain* terrainEntity;
};
/* Copyright © 2013 Fabian Schuiki */
#include "GameScene.h"
#include "Application.h"
#include <SFML/OpenGL.hpp>

GameScene::GameScene(Application *app) : Scene(app)
{
}

void GameScene::initialize()
{
}

bool GameScene::handleEvent(const sf::Event &event)
{
	
	return false;
}

void GameScene::advance(double dt)
{
	camera.azimuth += M_PI / 6 * dt;
	camera.advance(dt);
}

void GameScene::draw(const RenderInfo &info)
{
	glClear(GL_COLOR_BUFFER_BIT);

	// Update the camera viewport.
	camera.setViewport(info.width, info.height);
	camera.updateFrustum();
	camera.applyViewport();
	camera.apply();

	// Draw a triangle.
	glBegin(GL_TRIANGLES);
	glColor3f(1,0,0); glVertex3f(-1,-1,0);
	glColor3f(0,1,0); glVertex3f(1,-1,0);
	glColor3f(0,0,1); glVertex3f(0,1,0);
	glEnd();
}
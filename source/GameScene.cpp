/* Copyright © 2013 Fabian Schuiki */
#include "GameScene.h"
#include "Application.h"
#include <SFML/OpenGL.hpp>

GameScene::GameScene(Application *app) : Scene(app)
{
	wireframe = false;
	mouseDraggingRight = false;
}

void GameScene::initialize()
{
}

bool GameScene::handleEvent(const sf::Event &event)
{
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
		wireframe = !wireframe;
		return true;
	}
	if (event.type == sf::Event::MouseButtonPressed) {
		LOG(kLogDebug, "Mouse button %i pressed", event.mouseButton.button);
		// Adjust camera azimuth and inclination.
		if (event.mouseButton.button == sf::Mouse::Right) {
			mouse_x0 = event.mouseButton.x;
			mouse_y0 = event.mouseButton.y;
			mouseDraggingRight = true;
			return true;
		}

		// For debugging, calculate the near and far vertex of the clicked location.
		if (event.mouseButton.button == sf::Mouse::Left) {
			LOG(kLogDebug, "Clicked screen at %i x %i", event.mouseButton.x, event.mouseButton.y);
			clickRay = camera.unproject(event.mouseButton.x, event.mouseButton.y);
			LOG(kLogDebug, "Click ray %f x %f x %f towards %f x %f x %f", clickRay.p.x, clickRay.p.y, clickRay.p.z, clickRay.d.x, clickRay.d.y, clickRay.d.z);
		}
	}
	if (event.type == sf::Event::MouseButtonReleased) {
		LOG(kLogDebug, "Mouse button %i released", event.mouseButton.button);
		if (event.mouseButton.button == sf::Mouse::Right) {
			mouseDraggingRight = false;
			return true;
		}
	}
	if (event.type == sf::Event::MouseMoved) {
		if (mouseDraggingRight) {
			int dx = event.mouseMove.x - mouse_x0;
			int dy = event.mouseMove.y - mouse_y0;
			mouse_x0 = event.mouseMove.x;
			mouse_y0 = event.mouseMove.y;
			camera.azimuth += dx * 0.001 * M_PI;
			camera.inclination = std::max(M_PI / 12, std::min(M_PI / 2.1, camera.inclination + dy * 0.001 * M_PI));
			return true;
		}
	}
	return false;
}

void GameScene::advance(double dt)
{
	//camera.azimuth += M_PI / 6 * dt;
	camera.advance(dt);
}

void GameScene::draw(const RenderInfo &info)
{
	glClear(GL_COLOR_BUFFER_BIT);
	if (wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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

	terrainRenderer.draw(info);

	if (wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Draw the click ray.
	vec3 p1 = clickRay.p;
	vec3 p2 = clickRay.p + clickRay.d * camera.far;
	glColor3f(1,0,0);
	glBegin(GL_LINES);
	glVertex3f(p1.x, p1.y, p1.z);
	glVertex3f(p2.x, p2.y, p2.z);
	glEnd();
}
/* Copyright © 2013 Fabian Schuiki */
#include "GameScene.h"
#include "Application.h"
#include "Terrain.h"
#include "TerrainRenderer.h"
#include "ui/Manager.h"
#include "ui/Window.h"
#include "ConsoleWindow.h"
#include "BoundingVolume.h"
#include "WorldTerrain.h"
#include "model/Construction.h"
#include <SFML/OpenGL.hpp>
#include <fstream>
#include <cairomm/context.h>

GameScene::GameScene(Application *app) : Scene(app)
{
	wireframe = false;
	mouseDraggingRight = false;

	// Create terrain for testing purposes.
	terrain = new Terrain(200, 200);
	terrainRenderer = new TerrainRenderer;
	terrainRenderer->setTerrain(terrain);

	// Initialize the terrain entity.
	terrainEntity = new WorldTerrain;
	terrainEntity->setTerrain(terrain);

	// Initialize the UI manager.
	ui = new ui::Manager(app);

	// Initialize the console window.
	console = new ConsoleWindow(ui);
}

void GameScene::initialize()
{
	// Load heightmap for debugging.
	std::ifstream fin("assets/heightmap.raw");
	const double ah = 0.01;
	for (int y = 0; y < 200; y++) {
		for (int x = 0; x < 200; x++) {
			unsigned char a;
			fin.read((char *)&a, 1);
			TerrainNode &n = terrain->nodes[y * 200 + x];
			n.elevation = a * ah;
			n.gray = (double)a / 255;
		}
	}
	fin.close();
	terrain->analyze();

	// Update the terrain renderer.
	terrainRenderer->update();

	// Initialize the camera.
	camera.far = 1000;

	// Create some random empty window.
	ui::Window* w = new ui::Window(ui);
	w->x = 50;
	w->y = 50;
	w->resize(400,300);
	
	Cairo::RefPtr<Cairo::Context> cr = Cairo::Context::create(w->store);

    cr->save(); // save the state of the context
    cr->set_source_rgb(0.86, 0.85, 0.47);
    cr->paint(); // fill image with the color
    cr->restore(); // color is back to black now

    cr->save();
    // draw a border around the image
    cr->set_line_width(20.0); // make the line wider
    cr->rectangle(0.0, 0.0, w->store->get_width(), w->store->get_height());
    cr->stroke();

    cr->move_to(40, 40);
	cr->set_source_rgb(0.8, 0.2, 0.2);
	/*Cairo::RefPtr<Cairo::ToyFontFace> font =
	Cairo::ToyFontFace::create("Bitstream Charter",
	                           Cairo::FONT_SLANT_ITALIC,
	                           Cairo::FONT_WEIGHT_BOLD);
	cr->set_font_face(font);*/
	cr->set_font_size(32);
	cr->show_text("cairomm!");

	w->loadTexture();

	// Setup OpenGL lights.
	GLfloat ambientLight[]={0.1,0.1,0.1,1.0};    	             // set ambient light parameters
	glLightfv(GL_LIGHT0,GL_AMBIENT,ambientLight);

	GLfloat diffuseLight[]={0.9,0.9,0.9,1.0};    	             // set diffuse light parameters
	glLightfv(GL_LIGHT0, GL_DIFFUSE,diffuseLight);

	GLfloat specularLight[]={0.0,0.0,0.0,1.0};  	               // set specular light parameters
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);


	glEnable(GL_LIGHT0);                         	              // activate light0
	//glEnable(GL_LIGHTING);                       	              // enable lighting
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight); 
	glEnable(GL_COLOR_MATERIAL);                 	              // activate material
	glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);

	// Kick-start the simulation.
	simulation.start();

	// Schedule headquarters construction.
	model::Construction c;
	c.buildingName = "hq";
	c.around = vec3(105, 0, 60);
	simulation.scheduleConstruction(c);

	// Setup the command line interface.
	cli.add(ConsoleCommand<GameScene>::make(this, &GameScene::cli_bounds, "bounds", "<mask>...",
		"Configure rendering of bounding volumes.\n"
		"<mask> may be \"+name\" to add, \"-name\" to remove or \"name\" to toggle bounds. The following names are available:\n"
		"  all           All possible bounding volumes.\n"
		"  terrain       Bounds of terrain chunks.\n"
		"  terrain.cell  Bounds of individual terrain cells."));
	cli.add(ConsoleCommand<GameScene>::make(this, &GameScene::cli_normals, "normals", "<mask>...",
		"Configure rendering of normals.\n"
		"<mask> may be \"+name\" to add, \"-name\" to remove or \"name\" to toggle normals. The following names are available:\n"
		"  all           All possible normals.\n"
		"  terrain       All terrain normals.\n"
		"  terrain.node  Normals of the terrain nodes (vertices).\n"
		"  terrain.cell  Normals of the terrain cells (triangles)."));
	cli.add(ConsoleCommand<GameScene>::make(this, &GameScene::cli_construct, "construct", "<item>",
		"Select tool for construction.\n"
		"<item> may be one of the following:\n"
		"  hq  Headquarters"));

	// Initialize the model for debugging purposes.
	model = new WorldModel;
	model->setModel(ObjMeshAsset::get("Circular House.obj"));
}

bool GameScene::handleEvent(const sf::Event &event)
{
	// If the console window is visible, it receives all the events.
	if (console->isVisible() && console->handleEvent(event))
		return true;

	if (event.type == sf::Event::KeyPressed) {
		switch (event.key.code) {
			case sf::Keyboard::Space:
				wireframe = !wireframe;
				return true;
			case sf::Keyboard::Tab:
				console->show();
				return true;
			default: break;
		}
	}
	if (event.type == sf::Event::MouseButtonPressed) {
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
			TerrainCell *clickedCell = terrainEntity->findClickedCell(clickRay);

			// Do construction.
			if (!buildingName.empty()) {
				LOG(kLogDebug, "Attempt to construct building %s", buildingName.c_str());
			}
		}
	}
	if (event.type == sf::Event::MouseButtonReleased) {
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
	if (event.type == sf::Event::MouseWheelMoved) {
		camera.distance = std::max(sqrt(10), std::min(sqrt(400), camera.distance + (double)event.mouseWheel.delta * 0.1));
		return true;
	}
	if (event.type == sf::Event::TextEntered) {
		vec3 cx(-cos(camera.azimuth), 0, sin(camera.azimuth));
		vec3 cy( sin(camera.azimuth), 0, cos(camera.azimuth));
		double d = 0.1 * camera.distance * camera.distance;
		switch (event.text.unicode) {
			case 'w': camera.poi -= cy * d; return true;
			case 's': camera.poi += cy * d; return true;
			case 'a': camera.poi += cx * d; return true;
			case 'd': camera.poi -= cx * d; return true;
		}
	}
	return false;
}

void GameScene::advance(double dt)
{
	camera.poi.y = terrain->getElevation(camera.poi.x, camera.poi.z);
	camera.advance(dt);
}

void GameScene::draw(const RenderInfo &rootInfo)
{
	info.width  = rootInfo.width;
	info.height = rootInfo.height;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	if (wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Update the camera viewport.
	camera.setViewport(info.width, info.height);
	camera.updateFrustum();
	camera.applyViewport();
	camera.apply();

	GLfloat lightPos[] = {0.75, 1, 0.5, 0};
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	// Add some fog.
	GLfloat fogDensity = 0.01;
	GLfloat fogColor[4] = {0.25, 0.75, 1, 1.0};
	glEnable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_EXP2);
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogf(GL_FOG_DENSITY, fogDensity);
	//glFogf(GL_FOG_START, 20);
	//glFogf(GL_FOG_END, 100);
	glHint(GL_FOG_HINT, GL_NICEST);


	// Draw a triangle.
	glBegin(GL_TRIANGLES);
	glColor3f(1,0,0); glVertex3f(-1,-1,0);
	glColor3f(0,1,0); glVertex3f(1,-1,0);
	glColor3f(0,0,1); glVertex3f(0,1,0);
	glEnd();

	//terrainRenderer->draw(info);

	// Draw the terrain entity.
	terrainEntity->updateChunksIfDirty();
	terrainEntity->updateBoundsIfDirty();
	terrainEntity->draw(info);

	// Draw the model.
	model->updateBufferIfDirty();
	for (int x = 0; x < 10; x++) {
		for (int y = 0; y < 10; y++) {
			glPushMatrix();
			glTranslatef(1 + x*2, 5, 1 + y*2);
			glScalef(0.4, 0.4, 0.4);
			model->draw(info);
			glPopMatrix();
		}
	}

	if (wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Draw a line to locate each building in the world.
	// ... implement this ...

	// Draw the user interface.
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, info.width, info.height, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	console->resize(info.width, console->height);
	ui->draw(info);
}

ConsoleCommandGroup GameScene::getConsoleCommands()
{
	ConsoleCommandGroup cmds;
	cmds.add(&cli);
	cmds.add(terrainEntity->getConsoleCommands());
	return cmds;
}

void GameScene::cli_bounds(ConsoleCall& cmd)
{
	for (ConsoleArgs::const_iterator ia = cmd.args.begin(); ia != cmd.args.end(); ia++) {
		const std::string& arg = *ia;
		if (arg.size() == 0)
			continue;
		char op = arg[0];
		std::string fn = (op != '-' && op != '+' ? arg : arg.substr(1));
		int mask = 0;
		if (fn == "terrain") mask = RenderInfo::kTerrainBounds;
		if (fn == "terrain.cell") mask = RenderInfo::kTerrainCellBounds;
		if (fn == "all") mask = RenderInfo::kAllBounds;
		if (op == '-')
			info.drawBounds &= ~mask;
		else if (op == '+')
			info.drawBounds |= mask;
		else
			info.drawBounds ^= mask;
	}
}

void GameScene::cli_normals(ConsoleCall& cmd)
{
	for (ConsoleArgs::const_iterator ia = cmd.args.begin(); ia != cmd.args.end(); ia++) {
		const std::string& arg = *ia;
		if (arg.size() == 0)
			continue;
		char op = arg[0];
		std::string fn = (op != '-' && op != '+' ? arg : arg.substr(1));
		int mask = 0;
		if (fn == "terrain") mask = RenderInfo::kTerrainNormals;
		if (fn == "terrain.cell") mask = RenderInfo::kTerrainCellNormals;
		if (fn == "terrain.node") mask = RenderInfo::kTerrainNodeNormals;
		if (fn == "all") mask = RenderInfo::kAllNormals;
		if (op == '-')
			info.drawNormals &= ~mask;
		else if (op == '+')
			info.drawNormals |= mask;
		else
			info.drawNormals ^= mask;
	}
}

void GameScene::cli_construct(ConsoleCall& cmd)
{
	if (cmd.args.size() != 1) return;
	buildingName = cmd.args[0];
}
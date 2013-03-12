/* Copyright © 2013 Fabian Schuiki */
#include "Manager.h"
#include "Window.h"
#include "../Logger.h"
#include "../Application.h"
using ui::Manager;
using ui::Window;


Manager::Manager(Application* app) : ApplicationObject(app)
{
}

void Manager::draw(const RenderInfo& info)
{
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Draw the visible windows.
	for (std::list<Window*>::reverse_iterator it = visible.rbegin(); it != visible.rend(); it++) {
		Window* w = *it;
		if (w->needsRedraw)
			w->redraw();
		glBindTexture(GL_TEXTURE_2D, w->texture.id);
		glColor3f(1,1,1);
		glBegin(GL_QUADS);
		glTexCoord2f(0,0); glVertex2f(w->x, w->y);
		glTexCoord2f(1,0); glVertex2f(w->x + w->width, w->y);
		glTexCoord2f(1,1); glVertex2f(w->x + w->width, w->y + w->height);
		glTexCoord2f(0,1); glVertex2f(w->x, w->y + w->height);
		glEnd();
	}
	glDisable(GL_TEXTURE_2D);
}

/**
 * @brief Adds a window to the manager.
 *
 * This function should automatically be called whenever a new instance of
 * Window is created. This makes sure all the windows are in one place.
 */
void Manager::addWindow(Window* w)
{
	windows.insert(w);
}

void Manager::removeWindow(Window* w)
{
	windows.erase(w);
	visible.remove(w);
	w->setVisible(false);
}

void Manager::showWindow(Window* w)
{
	visible.remove(w);
	visible.push_front(w);
	w->setVisible(true);
}

void Manager::hideWindow(Window* w)
{
	visible.remove(w);
	w->setVisible(false);
}
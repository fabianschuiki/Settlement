/* Copyright © 2013 Fabian Schuiki */
#include "Window.h"
#include "Manager.h"
using ui::Window;
using ui::Manager;

/**
 * Creates a new window inside the given Manager.
 */
Window::Window(Manager* m) : manager(m)
{
	// Setup the default window attributes.
	x = y = 0;
	width = 0;
	height = 0;

	// Add the window to the manager.
	manager->addWindow(this);
}

Window::~Window()
{
	dispose();
}

/**
 * Removes the window from the Manager and deallocates all resources. Call this
 * function if you're getting rid of a window, since the garbage collector
 * won't collect the window immediately.
 */
void Window::dispose()
{
	manager->removeWindow(this);
}

void Window::resize(unsigned int w, unsigned int h)
{
	if (w != width || h != height) {
		width = w;
		height = h;
		resizeStore(w,h);
	}
}

void Window::resizeStore(unsigned int w, unsigned int h)
{
	// Reinitialize the backing store for the new size.
	store = Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32, w, h);
}

void Window::loadTexture()
{
	glBindTexture(GL_TEXTURE_2D, texture.id);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, store->get_width(), store->get_height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, store->get_data());
}
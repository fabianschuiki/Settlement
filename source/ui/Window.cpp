/* Copyright © 2013 Fabian Schuiki */
#include "Window.h"
#include "Manager.h"
#include "../Logger.h"
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
	needsRedraw = true;
	visible = false;

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

void Window::show()
{
	manager->showWindow(this);
}

void Window::hide()
{
	manager->hideWindow(this);
}

void Window::resizeStore(unsigned int w, unsigned int h)
{
	// Reinitialize the backing store for the new size.
	store = Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32, w, h);

	// Now the window needs to be redrawn.
	needsRedraw = true;
}

void Window::loadTexture()
{
	glBindTexture(GL_TEXTURE_2D, texture.id);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, store->get_width(), store->get_height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, store->get_data());
	glBindTexture(GL_TEXTURE_2D, 0);
}

bool Window::handleEvent(const sf::Event& event)
{
	return false;
}

void Window::draw(Cairo::RefPtr<Cairo::Context> ctx)
{
	ctx->save();
    ctx->set_source_rgb(0.9, 0.9, 0.9);
    ctx->paint();
    ctx->restore();

    ctx->save();
    ctx->set_line_width(1.0);
    ctx->rectangle(0, 0, width, height);
    ctx->stroke();
    ctx->restore();
}

void Window::setNeedsRedraw()
{
	needsRedraw = true;
}

/**
 * Makes the window draw itself.
 */
void Window::redraw()
{
	LOG(kLogDebug, "Redrawing window %p", this);
	needsRedraw = false;
	Cairo::RefPtr<Cairo::Context> context = Cairo::Context::create(store);

	context->save();
	context->set_operator(Cairo::OPERATOR_CLEAR);
	context->paint();
	context->restore();

	draw(context);
	loadTexture();
}

bool Window::isVisible() const
{
	return visible;
}

void Window::setVisible(bool v)
{
	visible = v;
}
/* Copyright © 2013 Fabian Schuiki */
#include "Manager.h"
using ui::Manager;
using ui::Window;

/**
 * @brief Adds a window to the manager.
 *
 * This function should automatically be called whenever a new instance of
 * Window is created. This makes sure all the windows are in one place.
 */
void Manager::addWindow(Window* w)
{
	windows.insert(w);
	visible.push_front(w);
}

void Manager::removeWindow(Window* w)
{
	windows.erase(w);
	visible.remove(w);
}

void Manager::showWindow(Window* w)
{
}

void Manager::hideWindow(Window* w)
{
}
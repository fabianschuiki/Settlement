/* Copyright © 2013 Fabian Schuiki */
#pragma once
#include "ui/Window.h"
#include <string>

/**
 * @brief In-game console window.
 */

class ConsoleWindow : public ui::Window
{
public:
	ConsoleWindow(ui::Manager* m);

	virtual bool handleEvent(const sf::Event& event);
	virtual void draw(Cairo::RefPtr<Cairo::Context> ctx);

private:
	std::string input;
	void executeCommand(std::string cmd);
};
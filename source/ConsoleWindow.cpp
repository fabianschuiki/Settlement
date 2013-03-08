/* Copyright © 2013 Fabian Schuiki */
#include "ConsoleWindow.h"
#include "Logger.h"


ConsoleWindow::ConsoleWindow(ui::Manager* m) : ui::Window(m)
{
	resize(0, 24);
}

bool ConsoleWindow::handleEvent(const sf::Event& event)
{
	// Escape closes the console.
	if (event.type == sf::Event::KeyPressed) {
		switch (event.key.code) {
			case sf::Keyboard::Escape: {
				hide();
			} break;
			case sf::Keyboard::Return: {
				LOG(kLogDebug, "Executing command!");
				input.clear();
				setNeedsRedraw();
				hide();
			} break;
			case sf::Keyboard::Back: {
				int p = (int)input.length() - 1;
				if (p >= 0) {
					input.erase(p);
					setNeedsRedraw();
				}
			} break;
			default: break;
		}
		return true;
	}

	// Accept all text input.
	if (event.type == sf::Event::TextEntered) {
		input += event.text.unicode;
		setNeedsRedraw();
		return true;
	}

	return false;
}

void ConsoleWindow::draw(Cairo::RefPtr<Cairo::Context> ctx)
{
	ctx->save();
	ctx->set_source_rgba(0, 0, 0, 0.8);
	ctx->paint();
	ctx->set_source_rgba(1, 1, 1, 0.3);
	ctx->move_to(0, height - 0.5);
	ctx->line_to(width, height - 0.5);
	ctx->set_line_width(1);
	ctx->stroke();
	ctx->restore();

	ctx->move_to(10, 16);
	ctx->set_source_rgb(1, 1, 1);
	ctx->select_font_face("Menlo", Cairo::FONT_SLANT_NORMAL, Cairo::FONT_WEIGHT_NORMAL);
	ctx->set_font_size(13);
	ctx->show_text(input);
}
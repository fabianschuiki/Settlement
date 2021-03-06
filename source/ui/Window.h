/* Copyright © 2013 Fabian Schuiki */
#pragma once
#include <gc_cpp.h>
#include "../gl/Texture.h"
#include <cairomm/surface.h>
#include <cairomm/context.h>
#include <SFML/Window/Event.hpp>

namespace ui
{
	class Manager;

	class Window : public gc
	{
		friend class Manager;
	public:
		Manager* const manager;

		Window(Manager *m);
		~Window();
		void dispose();
		void resize(unsigned int w, unsigned int h);

		virtual bool handleEvent(const sf::Event& event);
		virtual void draw(Cairo::RefPtr<Cairo::Context> ctx);

		void setNeedsRedraw();
		void redraw();

		bool isVisible() const;

		void show();
		void hide();

	protected:
		bool visible;
		void setVisible(bool v);

	public:
		gl::Texture texture;
		int x, y;
		unsigned int width, height;

		void loadTexture();

	public:
		Cairo::RefPtr<Cairo::ImageSurface> store;
		void resizeStore(unsigned int w, unsigned int h);
		bool needsRedraw;
	};
}
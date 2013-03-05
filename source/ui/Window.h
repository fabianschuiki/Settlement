/* Copyright © 2013 Fabian Schuiki */
#pragma once
#include <gc_cpp.h>
#include "../gl/Texture.h"
#include <cairomm/surface.h>

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

	public:
		gl::Texture texture;
		int x, y;
		unsigned int width, height;

		void loadTexture();

	public:
		Cairo::RefPtr<Cairo::ImageSurface> store;
		void resizeStore(unsigned int w, unsigned int h);
	};
}
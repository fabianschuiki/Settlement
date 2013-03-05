/* Copyright © 2013 Fabian Schuiki */
#pragma once
#include "../ApplicationObject.h"
#include <set>
#include <list>

class Application;
class RenderInfo;

namespace ui
{
	class Window;

	class Manager : public ApplicationObject
	{
		friend class Window;
	public:
		Manager(Application* app);
		void draw(const RenderInfo& info);

	protected:
		std::set<Window*> windows;
		void addWindow(Window* w);
		void removeWindow(Window* w);

		std::list<Window*> visible;
		void showWindow(Window* w);
		void hideWindow(Window* w);
	};
}
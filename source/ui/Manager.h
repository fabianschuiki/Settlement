/* Copyright © 2013 Fabian Schuiki */
#pragma once
#include <set>
#include <list>

namespace ui
{
	class Window;
	
	class Manager
	{
		friend class Window;
	public:

	protected:
		std::set<Window*> windows;
		void addWindow(Window* w);
		void removeWindow(Window* w);

		std::list<Window*> visible;
		void showWindow(Window* w);
		void hideWindow(Window* w);
	};
}
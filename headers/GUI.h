#pragma once

#include "GUIClasses.h"

namespace gui {
	// mouse pos
	extern sf::Vector2i mouse;
	extern sf::Vector2i mouseLast;
	// channels display (access for recalculation)
	extern Channels channels;
	// gui resizing
	extern bool cursorDragStarted;
	extern bool guiScalingStarted;
}

namespace gui {
	void setup(); // one-time setup
	void init(bool resize, bool recreate); // GUI scaling
	void update(); // GUI loop
	void render(); // GUI rendering
}
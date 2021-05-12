#ifndef GUI_H
#define GUI_H

#include "GUIClasses.h"

namespace gui {
	// channels display (access for recalculation)
	extern Channels channels;
	// gui resizing
	extern bool cursorDragStarted;
	extern bool guiScalingStarted;

	void setup(); // one-time setup
	void init(bool resize, bool recreate); // GUI scaling
	void update(); // GUI loop
	void render(); // GUI rendering
}

#endif
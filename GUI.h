#ifndef GUI_H
#define GUI_H

#include "GUIClasses.h"

namespace gui {
	// gui resizing
	extern bool resizeStarted;

	void setup(); // one-time setup
	void init(bool resize, bool recreate); // GUI scaling
	void update(); // GUI loop
	void render(); // GUI rendering
}

#endif
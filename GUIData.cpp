#include "GUIData.h" // interface
#include <iostream>

namespace gui {
	spacing::spacing(float Scale) {
		scale = Scale;
		boxW = (int)floor(float(boxW) * scale);
		boxH = (int)floor(float(boxH) * scale);
		space = (int)floor(float(space) * scale);
		pat = (int)floor(float(pat) * scale);
		patSpace = (int)floor(float(patSpace) * scale);
		patFull = (int)floor(float(patFull) * scale);
	}

	// uint8_t range : 0-255
	// usage : theme[c::bg] NOT theme[1]
	// for code readability
	unsigned char theme[][3]
	{
		{111, 116, 125},	// bg
		{133, 139, 150},	// logo1
		{78, 82, 89},		// logo2
		{102, 105, 116},	// inside
		{63, 66, 71},		// outside
		{37, 37, 37},		// outline
		{48, 48, 50},		// blank
		{132, 136, 150},	// input
		{255, 255, 255},	// light1
		{132, 136, 150},	// light2
		{0, 0, 0},			// dark
		{76, 102, 204},		// selection
		{100, 100, 100},	// keyC
		{75, 75, 75},		// keyG
		{255, 255, 255},	// clearWhite
	};

	// default spacing
	spacing cs = spacing(1.f);
	
}
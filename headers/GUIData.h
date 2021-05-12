#ifndef GUIDATA_H
#define GUIDATA_H

#include "GUIUtility.h"

#include <SFML/Graphics.hpp>

// structs, enums
namespace gui {
	// ui theme
	struct theme {
		// background
		sf::Color background = sf::Color(111, 116, 125);
		// widgets
		sf::Color boxOutline = sf::Color(37, 37, 37);
		sf::Color boxOutside = sf::Color(63, 66, 71);
		sf::Color boxInside = sf::Color(102, 105, 116);
		// buttons
		sf::Color buttonActive = sf::Color(255, 255, 255);
		sf::Color buttonInactive = sf::Color(102, 105, 116);

		theme();
		theme(std::string name);
	};

	// ui resizing
	struct scaling {
		float scale = 1.f;

		static const int boxConst = 240;
		static const int spaceConst = 6;
		static const int patConst = 30;
		static const int patSpaceConst = 3;

		int boxBoundsScaled = boxConst;

		int boxW = boxConst;
		int boxH = boxConst;
		int space = spaceConst;
		int pat = patConst;
		int patSpace = patSpaceConst;
		int patFull = pat + patSpace;

		scaling(float _scale);
	};

	// uint8_t range : 0-255
	// usage : palette[c::bg] NOT palette[1]
	// for code readability
	extern unsigned char palette[][3];

	extern theme th;
	extern scaling cs;
}

#endif
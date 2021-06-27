#ifndef GUIDATA_H
#define GUIDATA_H

#include "GUIUtility.h"

#include <SFML/Graphics.hpp>

// structs, enums
namespace gui {
	// ui theme
	struct theme {
		// general
		sf::Color background = sf::Color(111, 116, 125);
		sf::Color outline = sf::Color(37, 37, 37);
		sf::Color transparent = sf::Color(0, 0, 0, 0);
		// misc
		sf::Color channelMuted = sf::Color(200, 0, 0);
		// widgets
		sf::Color boxOutside = sf::Color(63, 66, 71);
		sf::Color boxInside = sf::Color(102, 105, 116);
		// progress bars
		sf::Color progressBarBackground = sf::Color(63, 66, 71);
		sf::Color progressBarValue = sf::Color(255, 255, 255);
		// buttons
		sf::Color buttonActive = sf::Color(255, 255, 255);
		sf::Color buttonInactive = sf::Color(102, 105, 116);
		// modules
		sf::Color moduleBg = sf::Color(111, 116, 125);
		// channels
		sf::Color selectedBG = sf::Color(255, 255, 255, 10);
		sf::Color selected = sf::Color(255, 255, 255, 30);

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

		int boxBoundsScaled,
			topH, boxW, boxH,
			space, modSpace, pat, patSpace, patFull;

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
#ifndef GUIUTILITY_H
#define GUIUTILITY_H

#include "GUIData.h"

#include <SFML/Graphics.hpp>

namespace gui {
	bool hover(float mx, float my, float x, float y, float w, float h);

	float approach(float a, float b, float c);

	sf::Color color(c index, uint8_t alpha = 255);
	sf::Color hsv(float H, float S, float V);
}

#endif
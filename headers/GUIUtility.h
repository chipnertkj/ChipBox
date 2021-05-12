#ifndef GUIUTILITY_H
#define GUIUTILITY_H

#include "GUIData.h"

#include <SFML/Graphics.hpp>

namespace gui {
	bool hover(float mx, float my, float x, float y, float w, float h);

	float approach(float a, float b, float c);

	sf::Color withAlpha(sf::Color base, sf::Uint8 a); // rgb(a) + a -> rgba
	sf::Color hsv(float H, float S, float V); // hsv -> rgb
}

#endif
#pragma once
#include "InterfaceData.h"

// creates and returns an sfml color value using the current theme
sf::Color color(c index, uint8_t alpha)
{
	uint8_t i = (uint8_t)index;
	return sf::Color(theme[i][0], theme[i][1], theme[i][2], alpha);
}

// moves a towards b by c
float approach(float a, float b, float c)
{
	c *= 1.0f/dt;
	c /= 60.0f;
	if (a < b)
		a += c;
	if (a > b)
		a -= c;
	return a;
}
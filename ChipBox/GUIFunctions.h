#pragma once
#include "GUIData.h"

namespace gui {
	// creates and returns an sfml color value using the current theme
	sf::Color color(c index, uint8_t alpha)
	{
		uint8_t i = (uint8_t)index;
		return sf::Color(theme[i][0], theme[i][1], theme[i][2], alpha);
	}

	// moves a towards b by c
	float approach(float a, float b, float c)
	{
		if (!vsync) {
			c *= 1.0f / dt;
			c /= 60.0f;
		}
		if (a < b)
			a += c;
		if (a > b)
			a -= c;
		return a;
	}

	float lerp(float a, float b, float c)
	{
		return (a * (1.0f - c)) + (b * c);
	}

	void setV(bool b) {
		v = b ? 1.0f : 0.0f;
	}

	void smooth(const char * a, float b, float c) {
		val[a] = approach(val[a], b, abs(val[a] - b) / c);
	}

	bool button(float bx, float by, float bw, float bh, bool centered)
	{
		if (centered) {
			bx -= bw / 2;
			by -= bh / 2;
		}

		if (mouse.x > bx) {
			if (mouse.x < bx + bw) {
				if (mouse.y > by) {
					if (mouse.y < by + bh) {
						return true;
					}
				}
			}
		}
		return false;
	}
}
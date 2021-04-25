#include "GUIUtility.h" // interface
#include "GUIData.h"

#include <SFML/Graphics.hpp>

namespace gui {
	bool hover(float mx, float my, float x, float y, float w, float h) {
		if (mx >= x)
			if (mx <= x + w)
				if (my >= y)
					if (my <= y + h)
						return true;
		return false;
	}

	float approach(float a, float b, float c) {
		if (a < b)
			a += c;
		if (a > b)
			a -= c;
		return a;
	}

	sf::Color color(c index, uint8_t alpha) {
		uint8_t i = (uint8_t)index;
		return sf::Color(theme[i][0], theme[i][1], theme[i][2], alpha);
	}

	sf::Color hsv(float H, float S, float V) {
		float s = S / 100;
		float v = V / 100;
		float C = s * v;
		float X = C * (1 - abs(fmod(H / 60.0, 2) - 1));
		float m = v - C;
		float r, g, b;
		if (H >= 0 && H < 60) {
			r = C, g = X, b = 0;
		}
		else if (H >= 60 && H < 120) {
			r = X, g = C, b = 0;
		}
		else if (H >= 120 && H < 180) {
			r = 0, g = C, b = X;
		}
		else if (H >= 180 && H < 240) {
			r = 0, g = X, b = C;
		}
		else if (H >= 240 && H < 300) {
			r = X, g = 0, b = C;
		}
		else {
			r = C, g = 0, b = X;
		}
		int R = (r + m) * 255;
		int G = (g + m) * 255;
		int B = (b + m) * 255;
		return sf::Color(R, G, B);
	}
}
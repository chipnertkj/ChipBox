#include "GUIUtility.h" // interface
#include "GUIData.h"

#include <SFML/Graphics.hpp>

namespace gui {
	bool hover(int mx, int my, float x, float y, float w, float h) {
		if (mx >= x)
			if (mx <= x + w)
				if (my >= y)
					if (my <= y + h)
						return true;
		return false;
	}

	float approach(float a, float b, float c) {
		c = std::min(c, std::abs(a - b));
		if (a < b)
			a += c;
		if (a > b)
			a -= c;
		return a;
	}

	sf::Color withAlpha(sf::Color base, sf::Uint8 a) {
		base.a = a;
		return base;
	}


	sf::Color hsv(float H, float S, float V) {
		float s = S / 100.f;
		float v = V / 100.f;
		float C = s * v;
		float X = C * (1.f - abs(fmod(H / 60.0f, 2.f) - 1.f));
		float m = v - C;
		float r, g, b;
		if (H >= 0.f && H < 60.f) {
			r = C, g = X, b = 0.f;
		}
		else if (H >= 60.f && H < 120.f) {
			r = X, g = C, b = 0.f;
		}
		else if (H >= 120.f && H < 180.f) {
			r = 0.f, g = C, b = X;
		}
		else if (H >= 180.f && H < 240.f) {
			r = 0.f, g = X, b = C;
		}
		else if (H >= 240.f && H < 300.f) {
			r = X, g = 0.f, b = C;
		}
		else {
			r = C, g = 0.f, b = X;
		}
		int R = (r + m) * 255.f;
		int G = (g + m) * 255.f;
		int B = (b + m) * 255.f;
		return sf::Color(R, G, B);
	}
}
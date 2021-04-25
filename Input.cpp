#pragma warning (disable : 26812)
#include "Input.h" // interface
#include "Keybinds.h"

namespace input {
	bool clicked = false;
	float scrollDelta = 0.f;

	bool onClick() {
		if (clicked) {
			clicked = false;
			return true;
		}
		return false;
	}

	void keyPressed(sf::Keyboard::Key key) {
		if (keybinds(key)) return; // loop through custom keybinds
	}

	void keyReleased(sf::Keyboard::Key key) {}
	void mousePressed(sf::Mouse::Button button) {
		if (button == sf::Mouse::Left)
			clicked = true;
	}
	void mouseReleased(sf::Mouse::Button button) {}
	void textEntered(sf::Uint32 text) {}

	void wheelScrolled(float delta) {
		scrollDelta += delta;
	}
}
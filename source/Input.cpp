#pragma warning (disable : 26812)
#include "Input.h" // interface
#include "Keybinds.h"

namespace input {
	bool clickedLMB = false;
	bool clickedMMB = false;
	bool clickedRMB = false;
	float scrollDelta = 0.f;

	bool onClick(bool& clickedButton) {
		if (clickedButton) {
			clickedButton = false;
			return true;
		}
		return false;
	}

	void reset() {
		input::scrollDelta = 0.f;
		input::clickedLMB = false;
		input::clickedMMB = false;
		input::clickedRMB = false;
	}

	void keyPressed(sf::Keyboard::Key key) {
		if (keybinds(key)) return; // loop through custom keybinds
	}

	void keyReleased(sf::Keyboard::Key key) {}
	void mousePressed(sf::Mouse::Button button) {
		if (button == sf::Mouse::Left)
			clickedLMB = true;
		else if (button == sf::Mouse::Middle)
			clickedMMB = true;
		else if (button == sf::Mouse::Right)
			clickedRMB = true;
	}
	void mouseReleased(sf::Mouse::Button button) {}
	void textEntered(sf::Uint32 text) {}

	void wheelScrolled(float delta) {
		scrollDelta -= delta;
	}
}
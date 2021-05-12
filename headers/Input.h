#ifndef INPUT_H
#define INPUT_H

#include <SFML/Graphics.hpp>

namespace input {
	extern bool clickedLMB;
	extern bool clickedMMB;
	extern bool clickedRMB;
	extern float scrollDelta;

	bool onClick();
	void reset();
	void keyPressed(sf::Keyboard::Key key);
	void keyReleased(sf::Keyboard::Key key);
	void mousePressed(sf::Mouse::Button button);
	void mouseReleased(sf::Mouse::Button button);
	void wheelScrolled(float delta);
	void textEntered(sf::Uint32 text);
}

#endif
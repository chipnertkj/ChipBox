#include <SFML/Graphics.hpp>
#include <iostream>

#include "Interface.h"

sf::Event event;

void keyPressed(sf::Keyboard::Key key) {
	switch (key)
	{
	case sf::Keyboard::Key::F11:
		fullscreen = !fullscreen;
		initialize(true);
		break;
	}
}
void keyReleased(sf::Keyboard::Key key) {

}
void mousePressed(sf::Mouse::Button button) {

}
void mouseReleased(sf::Mouse::Button button) {

}
void textEntered(const char text) {

}

void handleEvents() {
	while (mainWindow.pollEvent(event)) {
		switch (event.type) {
		case sf::Event::EventType::Closed:
			mainWindow.close(); break;
		case sf::Event::KeyPressed:
			keyPressed(event.key.code); break;
		case sf::Event::KeyReleased:
			keyReleased(event.key.code); break;
		case sf::Event::MouseButtonPressed:
			mousePressed(event.mouseButton.button); break;
		case sf::Event::MouseButtonReleased:
			mouseReleased(event.mouseButton.button); break;
		case sf::Event::TextEntered:
			textEntered(event.text.unicode);
		}
	}
}

int main() {
	// delta time clock
	sf::Clock clock;
	// initialize the interface
	if (!loadShaders())
		return 1;
	initialize(true);

	// program loop
	while (mainWindow.isOpen()) {
		handleEvents();
		render();

		// reset delta time clock
		dt = clock.getElapsedTime().asSeconds();
		clock.restart();
	}

	// terminate
	return 0;
}
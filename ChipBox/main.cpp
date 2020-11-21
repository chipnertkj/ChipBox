#include <SFML/Graphics.hpp>
#include <iostream>

#include "GUI.h"
#include "Host.h"

// event handler
sf::Event event;

// keybinds
gui::Keybind kb_addChannel(sf::Keyboard::Key::W, true);

void keyPressed(sf::Keyboard::Key& key) {
	// keybinds
	if (kb_addChannel.Check(key)) {
		popup = popups::addChannel;
	}
	else {
		// hardcoded
		switch (key)
		{
		case sf::Keyboard::Key::F11:
			fullscreen = !fullscreen;
			gui::init(true);
			break;
		case sf::Keyboard::Key::Escape:
			if (popup != popups::none)
				popup = popups::none;
			break;
		}
	}
}

void keyReleased(sf::Keyboard::Key& key) {

}

void mousePressed(sf::Mouse::Button& button) {
	if (hover != hovers::none) {
		switch (hover) {
		case hovers::addBox:
			popup = popups::addChannel;
			break;
		}
	}
}

void mouseReleased(sf::Mouse::Button& button) {

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
	// initialize the host

	// initialize the interface
	if (!gui::load())
		return 1;	// resource loading failed
	gui::init(true);

	// program loop
	while (mainWindow.isOpen()) {
		handleEvents();
		gui::update();
		gui::render();

		// reset delta time clock
		dt = clock.getElapsedTime().asSeconds();
		clock.restart();
	}

	// terminate
	return 0;	// executed
}
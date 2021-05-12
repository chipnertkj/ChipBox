#ifndef KEYBINDS_H
#define KEYBINDS_H

#include <SFML/Graphics.hpp>

namespace input {
	// keybind commands
	enum class cmd;

	class Keybind {
	public:
		const cmd command;
		sf::Keyboard::Key key;
		bool ctrl = false;
		bool shift = false;
		bool alt = false;

		bool check(sf::Keyboard::Key pressed);
	};

	// iterate through all keybinds and execute keybind commands
	bool keybinds(sf::Keyboard::Key key);

}

#endif
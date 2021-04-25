#pragma warning (disable : 26812)
#include "Keybinds.h" // interface
#include "App.h"
#include "GUI.h"

namespace input {
	bool Keybind::check(sf::Keyboard::Key pressed)
	{
		if (pressed != key)	// key
			return false;
		if (ctrl) {	 // ctrl
			if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RControl)))
				return false;
		}
		else {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RControl))
				return false;
		}
		if (shift) { // shift
			if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RShift)))
				return false;
		}
		else {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RShift))
				return false;
		}
		if (alt) {	 // alt
			if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LAlt) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RAlt)))
				return false;
		}
		else {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LAlt) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RAlt))
				return false;
		}
		return true; // success
	}

	// keybind commands
	enum class cmd {
		none = -1,
		fullscreen = 0,
		addChannel,
		console,
	};

	// all keybinds
	Keybind keybind[] = {
		Keybind {cmd::addChannel, sf::Keyboard::W, true },
		Keybind {cmd::fullscreen, sf::Keyboard::F11 },
		Keybind {cmd::console, sf::Keyboard::F3 },
	};
	const int keybindN = sizeof(keybind) / sizeof(Keybind);

	// iterate through all keybinds and execute keybind commands
	bool keybinds(sf::Keyboard::Key key) {
		cmd command = cmd::none;
		for (int i = 0; i < keybindN; i++) {
			if (keybind[i].check(key)) {
				command = keybind[i].command;
				break;
			}
		}

		switch (command) {
		default: break;
		case cmd::addChannel:
			app::project.addChannel("Channel " + std::to_string(app::project.channels.size()));
			return true;
		case cmd::fullscreen:
			app::fullscreen = !app::fullscreen;
			app::windowInit(true, false);
			return true;
		case cmd::console:
			app::console();
			return true;
		}

		return false;
	}
}
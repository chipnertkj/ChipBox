#pragma warning (disable : 26812)
#include "Keybinds.h" // interface
#include "App.h"
#include "GUI.h"
#include "GUIData.h"

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
		scaleDown,
		scaleUp,
		decreaseSongLength,
		increaseSongLength,
		channelsMoveUp,
		channelsMoveDown,
		channelsMoveLeft,
		channelsMoveRight,
		channelsJumpTop,
		channelsJumpBottom,
		channelsJumpLeft,
		channelsJumpRight,
	};

	// all keybinds
	Keybind keybind[] = {
		Keybind {cmd::addChannel, sf::Keyboard::W, true },
		Keybind {cmd::fullscreen, sf::Keyboard::F11 },
		Keybind {cmd::console, sf::Keyboard::F3 },
		Keybind {cmd::scaleDown, sf::Keyboard::Hyphen, true },
		Keybind {cmd::scaleUp, sf::Keyboard::Equal, true },
		Keybind {cmd::decreaseSongLength, sf::Keyboard::Hyphen, false, false, true},
		Keybind {cmd::increaseSongLength, sf::Keyboard::Equal, false, false, true},
		Keybind {cmd::channelsMoveUp, sf::Keyboard::Up},
		Keybind {cmd::channelsMoveDown, sf::Keyboard::Down},
		Keybind {cmd::channelsMoveLeft, sf::Keyboard::Left},
		Keybind {cmd::channelsMoveRight, sf::Keyboard::Right},
		Keybind {cmd::channelsJumpTop, sf::Keyboard::Up, true},
		Keybind {cmd::channelsJumpBottom, sf::Keyboard::Down, true},
		Keybind {cmd::channelsJumpLeft, sf::Keyboard::Left, true},
		Keybind {cmd::channelsJumpRight, sf::Keyboard::Right, true},
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
		default: return false;
		case cmd::addChannel:
			proj::project.addChannel("Channel " + std::to_string(proj::project.getChannelCount()), proj::ChannelType::empty);
			return true;
		case cmd::fullscreen:
			app::fullscreen = !app::fullscreen;
			app::windowInit(true, false);
			return true;
		case cmd::console:
			app::console();
			return true;
		case cmd::scaleDown:
			gui::cs = gui::scaling(std::max(0.8f, gui::cs.scale - 0.1f));
			gui::init(false, true);
			return true;
		case cmd::scaleUp:
			gui::cs = gui::scaling(std::min(1.3f, gui::cs.scale + 0.1f));
			gui::init(false, true);
			return true;
		case cmd::decreaseSongLength:
			if (proj::project.getSongLength() > 1)
				proj::project.setSongLength(proj::project.getSongLength()-1);
			return true;
		case cmd::increaseSongLength:
			proj::project.setSongLength(proj::project.getSongLength() + 1U);
			return true;
		case cmd::channelsMoveUp:
			gui::channels.moveSelected(0, -1);
			return true;
		case cmd::channelsMoveDown:
			gui::channels.moveSelected(0, 1);
			return true;
		case cmd::channelsMoveLeft:
			gui::channels.moveSelected(-1, 0);
			return true;
		case cmd::channelsMoveRight:
			gui::channels.moveSelected(1, 0);
			return true;
		case cmd::channelsJumpTop:
			gui::channels.setSelected(gui::channels.getSelected().x, 0);
			return true;
		case cmd::channelsJumpBottom:
			gui::channels.setSelected(gui::channels.getSelected().x, proj::project.getChannelCount() - 1);
			return true;
		case cmd::channelsJumpLeft:
			gui::channels.setSelected(0, gui::channels.getSelected().y);
			return true;
		case cmd::channelsJumpRight:
			gui::channels.setSelected(proj::project.getSongLength() - 1, gui::channels.getSelected().y);
			return true;
		}
	}
}
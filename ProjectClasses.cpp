#include "ProjectClasses.h" // interface

#include <iostream>

namespace proj {
	//---------------------------------------------------------
	// Project
	Channel* Project::addChannel(std::string name) {
		Channel* ch = new Channel(name, songLength);
		channels.push_back(ch);

		std::cout << "Adding channel \"" << name << "\"\n";
		return ch;
	}

	//---------------------------------------------------------
	// Channel
	Channel::Channel(std::string& Name, unsigned int len) {
		name = Name;
		for (size_t i = 0; i < len; i++)
			slots.push_back(0);
	}
}
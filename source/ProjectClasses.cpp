#include "ProjectClasses.h" // interface
#include "GUI.h"
#include "App.h"

namespace proj {
	//---------------------------------------------------------
	// Project
	unsigned int Project::getSongLength() {
		return songLength;
	}

	void Project::setSongLength(unsigned int length) {
		songLength = length;
		gui::channels.recalculate();
		gui::channels.scrollPanel->scroll.x = length + 1U;

		// add missing slots
		while (length > project.channels[0]->slots.size()) {
			for (auto it : project.channels)
				it->slots.push_back(0);
		}
		// remove obsolete slots
		while (length < project.channels[0]->slots.size()) {
			for (auto it : project.channels)
				it->slots.pop_back();
		}
	}

	Channel* Project::addChannel(std::string name) {
		app::cslog("PROJECT", "Adding channel \"" + name + "\"");
		// add channel to project
		Channel* ch = new Channel(name, songLength);
		channels.push_back(ch);
		// gui update
		gui::channels.recalculate();
		gui::channels.scrollPanel->scroll.y = project.channels.size() + 1U;
		return ch;
	}

	//---------------------------------------------------------
	// Channel
	Channel::Channel(std::string& Name, unsigned int length) {
		name = Name;
		for (unsigned int i = 0u; i < length; i++)
			slots.push_back(i + project.channels.size()* length);
	}
}
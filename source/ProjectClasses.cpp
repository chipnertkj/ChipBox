#include "ProjectClasses.h" // interface
#include "GUI.h"
#include "App.h"

namespace proj {
	//---------------------------------------------------------
	// Project
	unsigned int Project::getSongLength() {
		return songLength;
	}

	void Project::setSongLength(unsigned int _length) {
		// set
		songLength = _length;
		// gui update
		gui::channels.recalculate();
		gui::channels.scrollPanel->scroll.x = _length + 1U;
		// add missing slots
		while (_length > project.channels[0]->slots.size()) {
			for (auto it : project.channels)
				it->slots.push_back(0);
		}
		// remove obsolete slots
		while (_length < project.channels[0]->slots.size()) {
			for (auto it : project.channels)
				it->slots.pop_back();
		}
	}

	Channel* Project::addChannel(std::string _name, ChannelType _type) {
		app::cslog("PROJECT", "Adding channel \"" + _name + "\"");
		// add channel to project
		Channel* channel = new Channel(_name, _type, songLength);
		channels.push_back(channel);
		// gui update
		gui::channels.recalculate();
		gui::channels.scrollPanel->scroll.y = project.channels.size() + 1U;
		// return pointer
		return channel;
	}

	//---------------------------------------------------------
	// Channel
	Channel::Channel(std::string& _name, ChannelType _type, unsigned int _length) {
		// set
		name = _name;
		type = _type;
		// fill slots
		for (unsigned int i = 0u; i < _length; i++)
			slots.push_back(i + project.channels.size() * _length);
	}

	Instrument* Channel::addInstrument(std::string _name) {
		Instrument* instrument = NULL;
		switch (type) {
		case ChannelType::synth: {
			instrument = new InstrumentSynth(this, std::string(""));
			break;
		}
		}
		return instrument;
	}
}
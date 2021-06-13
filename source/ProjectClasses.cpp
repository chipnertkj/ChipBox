#include "ProjectClasses.h" // interface
#include "GUI.h"
#include "App.h"

#include <fstream>
#include <sstream>

namespace proj {
	//---------------------------------------------------------
	// Project
	Project::Project() {

	}

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
		instruments.push_back(new InstrumentSynth(this, "abc"));
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

	void Channel::process(chunk _chunk) {

	}

	//---------------------------------------------------------
	// Chip
	void Chip::allocate(unsigned int _count) {
		samples = new double[_count];
		count = _count;
	}
	void Chip::deallocate(unsigned int _count) {
		delete[] samples;
		count = 0;
	}
	void Chip::save(std::string name) {

	}

	//---------------------------------------------------------
	// Instrument

	//---------------------------------------------------------
	// InstrumentSynth
	std::string InstrumentSynth::customChipPath = "resources\\chips\\custom\\";
	const std::string InstrumentSynth::defaultChipPath = "resources\\chips\\";
	const std::string InstrumentSynth::chipExt = ".cbchip";
	const std::string InstrumentSynth::bankExt = ".cbcbank";

	InstrumentSynth::InstrumentSynth(Channel* _channel, std::string _name) {
		// setup
		channel = _channel;
		name = _name;
		// load default chips
		loadBank("default");
		loadChip("square"); // testing

		// init parameters
		params.gain		= new Parameter<double>(this, "Gain", 0, 0, 1);
		params.attack	= new Parameter<double>(this, "Attack", 0, 0, 1);
		params.decay	= new Parameter<double>(this, "Decay", 0, 0, 1);
		params.sustain	= new Parameter<double>(this, "Sustain", 0, 0, 1);
		params.release	= new Parameter<double>(this, "Release", 0, 0, 1);
	}

	/*
		Serialized Chip structure:
		2	- (uint)	sample count
		3	- (double)	sample range
		5+	- (double)	data
	*/
	
	void InstrumentSynth::loadChip(std::string _name, bool _custom) {
		if (chips.find(_name) != chips.end()) {
			app::cslog("LOADING CHIP", _name + " already loaded!");
			return;
		}

		Chip* chip = NULL;

		std::string path;
		if (_custom)
			path = customChipPath;
		else
			path = defaultChipPath;
		path += _name + chipExt;

		app::cslog("LOADING CHIP", "Loading " + _name);
		std::ifstream chipFile(path);
		if (chipFile.is_open()) {
			std::string text = "";
			unsigned int line = 1;

			chip = new Chip();
			unsigned int count = 0;
			double range = 0;
			while (std::getline(chipFile, text)) {
				if (line == 2) {
					std::stringstream(text) >> count;
					chip->allocate(count);
				}
				if (line == 3)
					std::stringstream(text) >> range;
				if (line > 4) {
					std::stringstream(text) >> chip->samples[line - 5];
					chip->samples[line - 5] /= range;
					app::cslog("", std::to_string(chip->samples[line - 5]));
				}
				line++;
			}
			app::cslog("LOADING CHIP", "Loaded from " + path);
		}
		else
			app::cslog("LOADING CHIP", "Failed to load from " + path);

		chips[_name] = chip;
	}

	/*
		Serialized Bank structure:
		2		- (uint)	number of chips
		+ 1		-			empty line
		+ 1		- (string)	chip name
		+ 1		- (uint)	sample count
		+ 1		- (double)	sample range
		+ 1+	- (double)	data
		+ 1		-			empty line
		+ 1		- (string)	chip name
		...
	*/

	void InstrumentSynth::loadBank(std::string _name, bool _custom) {
		Chip* chip = NULL;

		std::string path;
		if (_custom)
			path = customChipPath;
		else
			path = defaultChipPath;
		path += _name + bankExt;

		app::cslog("LOADING BANK", "Loading " + _name);
		std::ifstream bankFile(path);
		if (bankFile.is_open()) {
			std::string text = "";
			unsigned int line = 1;
			unsigned int lineLastEnd = 1;

			std::string name = "";
			unsigned int ccount = 0;
			unsigned int scount = 0;
			double srange = 0;
			bool skip = false;
			while (std::getline(bankFile, text)) {
				if (line == 2) {
					std::stringstream(text) >> ccount;
					lineLastEnd = line;
				}
				if (line == lineLastEnd + 2) {
					chip = new Chip();
					name = text;
					if (chips.find(name) != chips.end()) {
						app::cslog("LOADING CHIP", name + " already loaded!");
						skip = true;
					}
					else {
						skip = false;
						app::cslog("LOADING CHIP", "Loading " + name);
					}
				}
				if (line == lineLastEnd + 3)
					if (!skip) {
						std::stringstream(text) >> scount;
						chip->allocate(scount);
					}
				if (line == lineLastEnd + 4)
					if (!skip)
						std::stringstream(text) >> srange;
				if (line >= lineLastEnd + 6) {
					if (!skip) {
						if (line - lineLastEnd - 6 < scount) {
							std::stringstream(text) >> chip->samples[line - lineLastEnd - 6];
							chip->samples[line - lineLastEnd - 6] /= srange;
							app::cslog("", std::to_string(chip->samples[line - lineLastEnd - 6]));
						}
						else {
							app::cslog("LOADING CHIP", "Loaded " + name + " from " + _name + bankExt);
							lineLastEnd = line-1;
							chips[name] = chip;
							chip = new Chip();
						}
					}
				}
				line++;
			}
			app::cslog("LOADING BANK", "Loaded from " + path);
		}
		else
			app::cslog("LOADING BANK", "Failed to load from " + path);
	}

	void InstrumentSynth::process(chunk _chunk) {

	}
}
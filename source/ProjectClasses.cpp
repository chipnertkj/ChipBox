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

	void Project::setSongLength(unsigned int _length) {
		// set
		songLength = _length;
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
		// gui update
		gui::channels.recalculate();
		gui::channels.setSelected(_length - 1, gui::channels.getSelected().y);
	}

	Channel* Project::addChannel(std::string _name, ChannelType _type) {
		cslogstr("PROJECT", "Adding channel \"" + _name + "\"")
		// add channel to project
		Channel* channel = new Channel(_name, _type, songLength);
		channels.push_back(channel);
		// gui update
		gui::channels.recalculate();
		gui::channels.setSelected(gui::channels.getSelected().x, channels.size()-1);
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
			slots.push_back(0);
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
	// ChipSampled
	void ChipSampled::allocate(unsigned int _count) {
		samples = new double[_count];
		sampleCount = _count;
	}
	void ChipSampled::deallocate(unsigned int _count) {
		delete[] samples;
		sampleCount = 0;
	}
	void ChipSampled::save(std::string name) {

	}

	//---------------------------------------------------------
	// ChipAutomated
	void ChipAutomated::allocate(unsigned int _count) {
		points = new point[_count];
		pointCount = _count;
	}
	void ChipAutomated::deallocate(unsigned int _count) {
		delete[] points;
		pointCount = 0;
	}
	void ChipAutomated::save(std::string name) {

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

		// init parameters
		params.gain		= new Parameter<double>(this, "Gain", 0, 0, 1);
		params.attack	= new Parameter<double>(this, "Attack", 0, 0, 1);
		params.decay	= new Parameter<double>(this, "Decay", 0, 0, 1);
		params.sustain	= new Parameter<double>(this, "Sustain", 0, 0, 1);
		params.release	= new Parameter<double>(this, "Release", 0, 0, 1);
	}

	/*
		Serialized Chip structure:
		1	-			empty line
		2	- (uint)	sample count
		3	- (double)	sample range
		4	-			empty line
		5+	- (double)	data

		Empty lines can be used as places for comments.
	*/
	
	void InstrumentSynth::loadChip(std::string _name, bool _custom) {
		// TODO: add automated chip loading
		if (chips.find(_name) != chips.end()) {
			cslogstr("LOADING CHIP", _name + " already loaded!")
			return;
		}

		ChipSampled* chip = NULL;

		std::string path;
		if (_custom)
			path = customChipPath;
		else
			path = defaultChipPath;
		path += _name + chipExt;

		cslogstr("LOADING CHIP", "Loading " + _name)
		std::ifstream chipFile(path);
		if (chipFile.is_open()) {
			std::string text = "";
			unsigned int line = 1;

			chip = new ChipSampled();
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
					cslogstr("", std::to_string(chip->samples[line - 5]))
				}
				line++;
			}
			cslogstr("LOADING CHIP", "Loaded from " + path)
		}
		else
			cslogstr("LOADING CHIP", "Failed to load from " + path)

		chips[_name] = (Chip*)chip;
	}

	/*
		Serialized Bank structure:
		1-2		-			empty line
		+ 1		- (string)	chip name
		+ 1		- (uint)	sample count
		+ 1		- (double)	sample range
		+ 1		-			empty line
		+ 1+	- (double)	data
		+ 1		-			empty line
		+ 1		- (string)	chip name
		+ 1		- (uint)	sample count
		...

		Empty lines can be used as places for comments.
	*/

	void InstrumentSynth::loadBank(std::string _name, bool _custom) {
		ChipSampled* chip = NULL;

		std::string path;
		if (_custom)
			path = customChipPath;
		else
			path = defaultChipPath;
		path += _name + bankExt;

		cslogstr("LOADING BANK", "Loading " + _name)
		std::ifstream bankFile(path);
		if (bankFile.is_open()) {
			std::string text = "";
			unsigned int line = 1;
			unsigned int lineLastEnd = 1;

			std::string name = "";
			unsigned int scount = 0;
			double srange = 0;
			bool skip = false;
			while (std::getline(bankFile, text)) {
				if (line == lineLastEnd + 2) {
					chip = new ChipSampled();
					name = text;

					if (chips.find(name) != chips.end()) {
						cslogstr("LOADING CHIP", name + " already loaded!")
						skip = true;
					}
					else {
						skip = false;
						cslogstr("LOADING CHIP", "Loading " + name)
					}
				}
				if (line == lineLastEnd + 3) {
					std::stringstream(text) >> scount;
					if (!skip)
						chip->allocate(scount);
				}
				if (line == lineLastEnd + 4)
						std::stringstream(text) >> srange;
				if (line >= lineLastEnd + 6) {
					if (line - lineLastEnd - 6 < scount-1) {
						if (!skip) {
							std::stringstream(text) >> chip->samples[line - lineLastEnd - 6];
							chip->samples[line - lineLastEnd - 6] /= srange;
						}
					}
					else {
						if (!skip) {
							cslogstr("LOADING CHIP", "Loaded " + name + " from " + _name + bankExt)
							chips[name] = chip;
						}
						lineLastEnd = line;
					}
				}
				line++;
			}
			cslogstr("LOADING BANK", "Loaded from " + path)
		}
		else {
			cslogstr("LOADING BANK", "Failed to load from " + path)
		}
	}

	void InstrumentSynth::process(chunk _chunk) {

	}
}
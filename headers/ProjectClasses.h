#pragma once
#include "ProjectData.h"

#include <vector>
#include <string>

namespace proj {
	// TODO: timestamp for audio processing
	struct chunk {
		unsigned int timelineSlot = 0U;
		double delta = 0.;
	};

	class Note {

	};

	class Pattern {

	};

	class Channel;

	// Instrument base class
	// For general audio processing
	class Instrument {
	public:
		Channel* channel = NULL; // owner
		std::string name = "";

		Instrument(Channel* _channel, std::string _name);

		// TODO: implement processing for instruments
		virtual void process(chunk _chunk);
	};

	// Data parameters for instruments
	// Linked to gui through sliders, graphs and other components in the instrument box
	template <class T>
	class Parameter {
	private:
		T value;

	public:
		Instrument* instrument = NULL; // owner

		T get();
		T set(T _value);

		Parameter(T _value);
	};

	// Default ChipBox synth
	class InstrumentSynth : public Instrument {
	public:
		InstrumentSynth(Channel* _channel, std::string _name);

		struct {
			// ADSR envelope params
			Parameter<double> attack;
		} params;
	};

	// Channel class containing all related song timeline data and audio processing units (instruments) of one type
	class Channel {
	public:
		std::string name = "";
		ChannelType type;

		std::vector<Instrument*> instruments;
		std::vector<Pattern*> patterns;
		std::vector<unsigned int> slots;

		Channel(std::string& _name, ChannelType _type, unsigned int _length);

		Instrument* addInstrument(std::string _name);

		// TODO: implement processing for channels
		void process(chunk _chunk);
	};

	class Project {
	private:
		unsigned int songLength = 32;
	public:
		std::vector<Channel*> channels;

		Project();

		// TODO: implement project methods
		void load(std::string _path); // load project
		void create(); // create clean project

		unsigned int getSongLength();
		void setSongLength(unsigned int _length);

		Channel* addChannel(std::string _name, ChannelType _type);
	};
}


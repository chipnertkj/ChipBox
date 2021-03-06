#pragma once
#include "ProjectData.h"

#include <vector>
#include <string>
#include <map>


namespace proj {
	// TODO: fix protected


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
	class Instrument;

	// Data parameters for instruments
	// Linked to gui through sliders, graphs and other components in the instrument box
	template <class T>
	class Parameter {
	protected:
		std::string name = "";
		T value;
		T min;
		T max;
	public:
		Instrument* instrument = NULL; // owner

		Parameter(Instrument* _instrument, std::string _name, T _value, T _min, T _max) {
			instrument = _instrument;
			name = _name;
			value = _value;
			min = _min;
			max = _max;
		}

		T set(T _value) {
			value = std::max(min, std::min(max, value));
		}

		T get() {
			return value;
		}
	};

	// A boolean Parameter
	class Switch {
	protected:
		std::string name = "";
		bool value;
	public:
		Instrument* instrument = NULL; // owner

		Switch(std::string _name, bool _value);
		bool set(bool _value);
		bool get() { return value; }
	};

	class Chip {
	protected:
		enum class types {
			none,
			sampled,	// uses preloaded chip samples
			automated,	// generates chip samples
		};
	public:
		virtual void save(std::string name) = NULL;
		virtual types getType() { return types::sampled; }
	};

	class ChipSampled : public Chip {
	protected:
		unsigned int sampleCount = 0;
	public:
		double* samples = NULL;
		void allocate(unsigned int _count);
		void deallocate(unsigned int _count);
		void save(std::string name) override final;
		types getType() override final { return types::sampled; }
	};

	class ChipAutomated : public Chip {
	protected:
		struct point { unsigned int x = 0, y = 0; };
		unsigned int pointCount = 0;
	public:
		point* points = NULL;
		void allocate(unsigned int _count);
		void deallocate(unsigned int _count);
		void save(std::string name) override final;
		types getType() override final { return types::automated; }

	};
	
	// Instrument base class
	// For general audio processing
	class Instrument {
	protected:
		Instrument() {};
	public:
		Channel* channel = NULL; // owner
		std::string name = "";

		// TODO: implement processing for instruments
		virtual void process(chunk _chunk) = NULL;
	};

	// Default ChipBox synth
	class InstrumentSynth : public Instrument {
	protected:
		// instrument settings
		std::string currentChip = "";
	public:
		// static stuff
		static const std::string defaultChipPath; // const path for chipbox chips
		static std::string customChipPath; // path for user defined chips
		static const std::string chipExt; // const chip extension
		static const std::string bankExt; // const bank extension

		inline static std::map<std::string, Chip*> chips;
		
		// deserializes and loads a chip, set 2nd param to true if it's user defined
		static void loadChip(std::string _path, bool _custom = false);
		// deserializes and loads a chip bank (a set of chips)
		static void loadBank(std::string _path, bool _custom = false);

		// parameters
		struct {
			// general
			Parameter<double>* gain;
			// ADSR envelope params
			Parameter<double>* attack;
			Parameter<double>* decay;
			Parameter<double>* sustain;
			Parameter<double>* release;
		} params;

		// methods
		InstrumentSynth(Channel* _channel, std::string _name);
		void process(chunk _chunk) override;
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
		unsigned int songLength = 8;
		std::vector<Channel*> channels;
	public:

		Project();

		// TODO: implement project methods
		void load(std::string _path); // load project
		void create(); // create clean project

		unsigned int getSongLength() { return songLength; }
		void setSongLength(unsigned int _length);

		Channel* addChannel(std::string _name, ChannelType _type);
		Channel* getChannel(unsigned int n) { return channels[n]; }
		unsigned int getChannelCount() { return channels.size(); }
	};
}


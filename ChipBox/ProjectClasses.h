#pragma once
#include "ProjectData.h"
#include <map>
#include <vector>

class Note {
public:
	Note() {}

	void setLength(int l) {
		length = l;
	}

	int length = 0;
};

class Pattern {
public:
	Pattern() {}

	std::map<int, Note> notes;
};

class Instrument {
public:
	Instrument() {}

	std::string name = "";
};

class Channel {
private:
	std::map<int, Pattern> patterns;
	std::vector<Instrument> instruments;
public:
	Channel() {}

	int slotCount = 0;
	int instrumentCount = 0;
	std::vector<int> slots;
	std::string name = "";
};

class Project {
private:
	std::vector<Channel> channels;
public:
	Project() {}
	// parameters
	int songLength = 32;
	int songPatterns = 32;
	int patternBars = 8;
	// values
	int channelCount = 0;
	std::string name = "";
};
#ifndef PROJECTCLASSES_H
#define PROJECTCLASSES_H

#include <vector>
#include <string>

namespace proj {
	class Note {

	};

	class Pattern {

	};

	class Instrument {

	};

	class Channel {
	private:

	public:
		std::string name = "";

		std::vector<Instrument*> instruments;
		std::vector<Pattern*> patterns;
		std::vector<unsigned int> slots;

		Channel(std::string& Name, unsigned int len);
	};

	class Project {
	private:
		unsigned int songLength = 32;
	public:

		std::vector<Channel*> channels;

		unsigned int getSongLength();
		void setSongLength(unsigned int length);

		Channel* addChannel(std::string name);
	};
}

#endif
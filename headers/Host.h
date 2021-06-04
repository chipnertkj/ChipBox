// Audio engine

#pragma once
#include <thread>

namespace host {
	extern std::thread* thread; // audio engine thread
}

namespace host {
	void start(); // start the audio engine
	void process(); 
	void stop(); // stop the audio engine
}
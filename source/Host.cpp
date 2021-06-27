#include "Host.h" // interface
#include "App.h"
#include "HostClasses.h"

#include "portaudio.h"

#include <chrono>

namespace host {
	std::thread* thread; // audio engine thread'
	bool running = false;

	// TODO: Implement base host classes https://developer.steinberg.help/display/VST/%5B3.0.0%5D+Interfaces+supported+by+the+host
}

namespace host {

	/*
	void loadPlugin(LPCWSTR dllPath) {
		// module loading
		HMODULE dll = LoadLibraryW(dllPath);
		if (dll == 0) {
			app::exception("Couldn't load the plugin module");
			return;
		}

		// module init
		auto initDll = (bool(*)())GetProcAddress(dll, "InitDll");
		if (initDll) { initDll(); }
		// module factory proc
		auto proc = (GetFactoryProc)GetProcAddress(dll, "GetPluginFactory");
		Steinberg::IPluginFactory* rawFactory = proc();

		// get factory info
		Steinberg::PFactoryInfo factoryInfo;
		rawFactory->getFactoryInfo(&factoryInfo);
		// get classes
		for (int i = 0; i < rawFactory->countClasses(); i++) {
			// get class info
			Steinberg::PClassInfo info;
			rawFactory->getClassInfo(i, &info);

			// skip if it's not a processor module (?)
			if (info.category != kVstAudioEffectClass)
				continue;
			// create class component
			Steinberg::Vst::IComponent* comp(nullptr);
			Steinberg::tresult result = rawFactory->createInstance(info.cid, Steinberg::Vst::IComponent::iid, (void**)&comp);
			if (result != Steinberg::kResultTrue) {
				app::exception("Couldn't create a plugin class component");
				return;
			}
			// initialize component
			comp->setIoMode(Steinberg::Vst::IoModes::kAdvanced);
			comp->initialize(hostApplication);

			// get edit controller
			Steinberg::TUID edit_cid;
			comp->getControllerClassId(edit_cid);
			// create edit controller
			Steinberg::Vst::IEditController* edit(nullptr);
			result = rawFactory->createInstance(edit_cid, Steinberg::Vst::IEditController::iid, (void**)&edit);
			if (result != Steinberg::kResultTrue) {
				app::exception("Couldn't create an edit controller");
				return;
			}
			// initialize edit controller
			edit->initialize(hostApplication);

			// free resources
			edit->terminate();
			comp->terminate();
			edit->release();
			comp->release();
			edit = NULL;
			comp = NULL;
		}
	}
	*/

	// host / audio engine initialization
	void init() {
		// PortAudio
		Pa_Initialize();
		cslogstr("PortAudio", std::string("Running ") + Pa_GetVersionInfo()->versionText)
		for (PaHostApiIndex i = 0; i < Pa_GetHostApiCount(); i++) {
			cslogstr("Available Host APIs", Pa_GetHostApiInfo(i)->name)
		}
	}

	void start() {
		init();

		running = true;
		thread = new std::thread(process);
	}

	void process() {
		cslogstr("AUDIO ENGINE", "Thread started")
		while (running) {
			std::this_thread::sleep_for(std::chrono::milliseconds(300)); // temp
			// TODO: stop when idle to decrease cpu usage cause the idiots will scream at me if i dont

		}
		cslogstr("AUDIO ENGINE", "Thread stopped")
	}

	void stop() {
		running = false;

	}
}
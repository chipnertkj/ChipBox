#include "App.h" // interface
#include "GUI.h"
#include "Input.h"

#include <Windows.h>
#include <iostream>

// #define CB_DEBUG enables debug logging
#define CB_DEBUG

namespace app {
	/// window
	std::string windowName = std::string("ChipBox | ") + __TIME__ + ", " + __DATE__;
	sf::RenderWindow mainWindow;
	gui::Render mainRender;
	sf::Image icon;
	sf::Event event;
	HWND mainWindowHandle;

	/// app data
	sf::VideoMode res = sf::VideoMode(0, 0);
	sf::VideoMode display = sf::VideoMode::getDesktopMode();
	bool fullscreen = false;
	bool cursorChanged = false;

	sf::VideoMode resNonFullscreen = res;
	sf::Vector2i posNonFullscreen = sf::Vector2i(-100000, -100000);
	sf::Clock clock; // dt timer
	float dt = 1;

	/// shaders
	sf::Shader shaderH;
	sf::Shader shaderV;
	bool blur = false;

	/// resources
	// textures
	sf::Texture tx_logo32;
	sf::Texture tx_logo256;
	sf::Texture tx_plus;
	sf::Texture tx_buttonHighlightBig;
	sf::Texture tx_dragHandle;
	// fonts
	sf::Font font;
	// cursors
	sf::Cursor cursor_arrow;
	sf::Cursor cursor_hand;
	sf::Cursor cursor_sizeH;
	sf::Cursor cursor_sizeV;
	sf::Cursor cursor_text;
	sf::Cursor cursor_tleftbright;
}

namespace app {
	// window init + resizing
	void windowInit(bool recreate, bool resize) {
		sf::VideoMode vm;
		if (!resize) {
			vm = sf::VideoMode::getDesktopMode();
			display.width = vm.width;
			display.width = vm.height;
		}
		else {
			sf::Vector2u s = mainWindow.getSize();
			vm.width = s.x;
			vm.height = s.y;
		}

		// window
		if (recreate) {
			if (!resize) {
				if (fullscreen) {
					posNonFullscreen = mainWindow.getPosition();
					vm.height++;
					mainWindow.create(vm, windowName, sf::Style::None);
				}
				else {
					if (resNonFullscreen.width == 0) {
						vm.width = (unsigned int)(vm.width * 0.8f);
						vm.height = (unsigned int)(vm.height * 0.8f);
					}
					else {
						vm.width = resNonFullscreen.width;
						vm.height = resNonFullscreen.height;
					}

					mainWindow.create(vm, windowName, sf::Style::Close | sf::Style::Resize);
					if (posNonFullscreen.x != -100000 && posNonFullscreen.y != -100000)
						mainWindow.setPosition(posNonFullscreen);
				}

				// set icon
				mainWindow.setIcon(64, 64, icon.getPixelsPtr());
			}
			else {
				sf::View view;
				view.reset(sf::FloatRect(0, 0, (float)vm.width, (float)vm.height));
				mainWindow.setView(view);
			}

			// (re)create renders
			mainRender.create(vm.width, vm.height);
			mainWindow.setVerticalSyncEnabled(true);

			// shader uniforms
			shaderH.setUniform("resolution", (float)vm.width);
			shaderV.setUniform("resolution", (float)vm.height);
		}

		mainWindowHandle = mainWindow.getSystemHandle();

		res.width = vm.width;
		res.height = vm.height;
		if (!fullscreen)
			resNonFullscreen = res;

		// interface setup
		gui::init(resize, recreate);
	}

	// window event handling
	void handleEvents() {
		while (mainWindow.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::Closed:
				mainWindow.close(); break;
			case sf::Event::Resized:
				windowInit(true, true); break;
			case sf::Event::KeyPressed:
				input::keyPressed(event.key.code); break;
			case sf::Event::KeyReleased:
				input::keyReleased(event.key.code); break;
			case sf::Event::MouseButtonPressed:
				input::mousePressed(event.mouseButton.button); break;
			case sf::Event::MouseButtonReleased:
				input::mouseReleased(event.mouseButton.button); break;
			case sf::Event::TextEntered:
				input::textEntered(event.text.unicode); break;
			case sf::Event::MouseWheelScrolled:
				input::wheelScrolled(event.mouseWheelScroll.delta);
			}
		}
	}

	// load resources
	bool load() {
		cslog("SYSTEM", "Loading resources...");

		// blur shader
		if (!shaderH.loadFromFile("resources/shaders/gaussianH.glsl", sf::Shader::Fragment))
			return false;
		if (!shaderV.loadFromFile("resources/shaders/gaussianV.glsl", sf::Shader::Fragment))
			return false;
		shaderH.setUniform("u_texture", sf::Shader::CurrentTexture);
		shaderV.setUniform("u_texture", sf::Shader::CurrentTexture);

		// icon
		if (!icon.loadFromFile("resources/images/icon64.png"))
			return false;

		/// textures
		// tx_logo32
		if (!tx_logo32.loadFromFile("resources/images/logo32.png"))
			return false;
		tx_logo32.setSmooth(true);
		// tx_logo256
		if (!tx_logo256.loadFromFile("resources/images/logo256.png"))
			return false;
		tx_logo256.setSmooth(true);
		// tx_plus
		if (!tx_plus.loadFromFile("resources/images/plus.png"))
			return false;
		// tx_buttonHighlightBig
		if (!tx_buttonHighlightBig.loadFromFile("resources/images/buttonHighlightBig.png"))
			return false;
		// tx_dragHandle
		if (!tx_dragHandle.loadFromFile("resources/images/dragHandle.png"))
			return false;
		tx_dragHandle.setSmooth(true);

		/// fonts
		if (!font.loadFromFile("resources/fonts/timeburnernormal.ttf"))
			return false;

		/// cursors
		cursor_arrow.loadFromSystem(sf::Cursor::Arrow);
		cursor_hand.loadFromSystem(sf::Cursor::Hand);
		cursor_sizeH.loadFromSystem(sf::Cursor::SizeHorizontal);
		cursor_sizeV.loadFromSystem(sf::Cursor::SizeVertical);
		cursor_text.loadFromSystem(sf::Cursor::Text);
		cursor_tleftbright.loadFromSystem(sf::Cursor::SizeTopLeftBottomRight);

		cslog("SYSTEM", logColorGreen("DONE"));

		return true;
	}

	// switch visibility for console output (debugging)
	void console() {
		HWND c = GetConsoleWindow();
		ShowWindow(c, IsWindowVisible(c) ? SW_HIDE : SW_SHOW);
		SetFocus(mainWindowHandle);
	}
	// set visibility for console output (debugging)
	void console(bool visibility) {
		HWND c = GetConsoleWindow();
		ShowWindow(c, visibility ? SW_HIDE : SW_SHOW);
		SetFocus(mainWindowHandle);
	}

	// console format green
	inline std::string logColorGreen(std::string text) {
		return "\033[92m" + text + "\033[0m";
	}

	// log exceptions
	void exception(std::string t) {
		console(true);
		std::cout << t << std::endl;
		system("pause");
	}
#ifdef CB_DEBUG
	// log events
	void cslog(std::string base, std::string text) {
		std::cout << "[" << base << "] " << text << std::endl;
	}
#else
	// log events
	void cslog(std::string base, std::string text) {
		return;
	}
#endif

	// MAIN INIT
	bool init() {
		cslog("SYSTEM", "Initializing.");

		// load resources
		if (!load())
			return false;

		// loading window
		sf::RenderWindow loadingWindow;
		loadingWindow.create(sf::VideoMode(300, 200), windowName, sf::Style::None);
		loadingWindow.clear();

		sf::Sprite logo;
		logo.setTexture(tx_logo256);
		logo.setOrigin(sf::Vector2f(tx_logo256.getSize() / 2U));
		logo.setScale(0.5, 0.5);
		logo.setPosition(150, 80);

		sf::Text text;
		text.setFont(font);
		text.setString("loading");
		text.setOrigin(sf::Vector2f{ text.getGlobalBounds().width/2, text.getGlobalBounds().height/2 });
		text.setPosition(150, 161);

		loadingWindow.draw(logo);
		loadingWindow.draw(text);
		loadingWindow.display();

		// load vsts

		// close loading window
		loadingWindow.close();

		// app setup
		gui::setup();
		windowInit(true, false);

#if defined(NDEBUG) || !defined(CB_DEBUG) // hide cmd in release and when logging is disabled
		console(false);
#endif

		return true;
	}

	// MAIN LOOP
	void run() {
		while (mainWindow.isOpen()) {
			// input
			if (cursorChanged) {
				mainWindow.setMouseCursor(cursor_arrow);
				cursorChanged = false;
			}

			// event handling
			handleEvents();

			// gui update
			if (mainWindowHandle == GetFocus())
				gui::update();
			
			// input reset
			input::reset();

			// gui render
			if (!IsIconic(mainWindowHandle))
				gui::render();
			else
				mainWindow.display(); // cpu usage sleep fix

			// timing
			dt = clock.getElapsedTime().asSeconds();
			clock.restart();
		}
	}

	// MAIN EXIT
	void exit() {
#ifdef _DEBUG // show cmd in debug
		ShowWindow(GetConsoleWindow(), SW_SHOW);
#endif
	}
}

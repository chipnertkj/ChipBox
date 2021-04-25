#ifndef APP_H
#define APP_H

#include "GUIClasses.h"
#include "Project.h"

#include <SFML/Graphics.hpp>
#include <Windows.h>

namespace app {
	/// window
	extern sf::RenderWindow mainWindow;
	extern gui::Render mainRender;
	extern HWND mainWindowHandle;

	/// app data
	extern sf::VideoMode res; // app resolution
	extern sf::VideoMode display; // display resolution
	extern bool fullscreen;
	extern bool cursorChanged;

	/// project data
	extern proj::Project project;

	/// shaders
	extern sf::Shader shaderH;
	extern sf::Shader shaderV;
	extern bool blur;

	/// resources
	// textures
	extern sf::Texture tx_logo32;
	extern sf::Texture tx_logo256;
	extern sf::Texture tx_plus;
	extern sf::Texture tx_buttonHighlightBig;
	extern sf::Texture tx_dragHandle;
	// fonts
	extern sf::Font font;
	// cursors
	extern sf::Cursor cursor_arrow;
	extern sf::Cursor cursor_hand;
	extern sf::Cursor cursor_sizeH;
	extern sf::Cursor cursor_sizeV;
	extern sf::Cursor cursor_text;
	extern sf::Cursor cursor_tleftbright;
}

namespace app {
	void windowInit(bool recreate, bool resize); // window init
	void handleEvents(); // window event handling
	bool load(); // load resources
	void console();

	bool init(); // MAIN INIT
	void run();  // MAIN LOOP
	void exit(); // MAIN EXIT
}

#endif
// remember to remove anything labeled as debug before releasing

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>	// debug
#include <string>
#include <map>
#include <algorithm>
#include <chrono>
#include "Discord.h"
#include <vector>

#define FILE "";

char filename[128] = "Untitled.chipbox";

#pragma region Discord RPC

void Discord::Initialize()
{
	DiscordEventHandlers Handle;
	memset(&Handle, 0, sizeof(Handle));
	Discord_Initialize("739852855338467420", &Handle, 1, NULL);
}

void Discord::Update()
{
	// filename workaround
#undef FILE
#define FILE filename;

// update
	DiscordRichPresence discordPresence;
	memset(&discordPresence, 0, sizeof(discordPresence));
	discordPresence.state = FILE;
	discordPresence.details = "Editor";
	std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
	std::chrono::system_clock::duration dtn = tp.time_since_epoch();
	discordPresence.startTimestamp = dtn.count() * std::chrono::system_clock::period::num / std::chrono::system_clock::period::den;
	discordPresence.largeImageKey = "icon";
	discordPresence.largeImageText = "ChipBox";
	discordPresence.smallImageKey = "logo512";
	discordPresence.smallImageText = FILE;
	Discord_UpdatePresence(&discordPresence);
}
#pragma endregion

#pragma region App Setup

Discord * g_Discord;

static const std::string title = "ChipBox";
static const std::string version = "devbuild 7.29.20";

static const int inputMax = 38;

std::string inputString;
int displayW = sf::VideoMode::getDesktopMode().width;
int displayH = sf::VideoMode::getDesktopMode().height;
int boxSize, pat;
int popup = -1;
int hover = -1;
int framerateLimit = 0;
float scale, out;
bool fullscreen = false;
bool pressed = false;
bool debug = false;
bool drawReset = true;
bool channelsUpdated = true;
bool vsync = true;
bool shaders = true;
bool typing = false;
int inputPos = 0;
float inputWait = 0.0f;
sf::Vector2f channelScroll(0.0f, 0.0f);
sf::Vector2f channelScrollApp(0.0f, 0.0f);
int songLength = 16;

sf::Int64 dt;	// delta time

sf::RenderWindow window(sf::VideoMode(300, 1), "Loading...");
sf::View view = window.getView();
sf::Event event;
sf::Vector2u windowSize = window.getSize();

sf::RenderTexture render;
sf::RenderTexture rt_channelScroll;
sf::RenderTexture rt_channelPanel;
sf::RenderTexture rt_pianoKeys;
sf::RenderTexture rt_pianoRoll;

sf::Vector2u channelScrollSize;
sf::Vector2u channelPanelSize;
sf::Vector2u pianoKeysSize;
sf::Vector2u pianoRollSize;

// usage : theme[c::bg] NOT theme[1]
// for code readability
enum c
{
	bg,
	logo1,
	logo2,
	inside,
	outside,
	outline,
	blank,
	input,
	light1,
	light2,
	dark,
	selection,
	keyC,
	keyG,
};


// uint8_t range : 0-255
// usage : theme[c::bg] NOT theme[1]
// for code readability
uint8_t theme[][3]
{
	{111, 116, 125},	// bg
	{133, 139, 150},	// logo1
	{78, 82, 89},		// logo2
	{102, 105, 116},	// inside
	{63, 66, 71},		// outside
	{37, 37, 37},		// outline
	{48, 48, 50},		// blank
	{132, 136, 150},	// input
	{255, 255, 255},	// light1
	{132, 136, 150},	// light2
	{0, 0, 0},			// dark
	{76, 102, 204},		// selection
	{100, 100, 100},	// keyC
	{75, 75, 75},		// keyG
};

enum channelType
{
	synth,
	noise,
	sample,
	mod
};

enum hovers
{
	addCh,
	renameCh,
	popupDone,
};

enum popups
{
	addCha,
	renameChannel
};

#pragma endregion

#pragma region Classes

// A customizable keybind
class Keybind {
public:
	sf::Keyboard::Key key;
	bool ctrl;
	bool shift;
	bool alt;

	Keybind(sf::Keyboard::Key Key)
	{
		key = Key;
		ctrl = false;
		shift = false;
		alt = false;
	};

	Keybind(sf::Keyboard::Key Key, bool Ctrl)
	{
		key = Key;
		ctrl = Ctrl;
		shift = false;
		alt = false;
	};

	Keybind(sf::Keyboard::Key Key, bool Ctrl, bool Shift)
	{
		key = Key;
		ctrl = Ctrl;
		shift = Shift;
		alt = false;
	};

	Keybind(sf::Keyboard::Key Key, bool Ctrl, bool Shift, bool Alt)
	{
		key = Key;
		ctrl = Ctrl;
		shift = Shift;
		alt = Alt;
	};

	bool Check(sf::Keyboard::Key pressed)
	{
		if (pressed != key)
			return false;
		if (ctrl) {
			if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RControl)))
				return false;
		}
		else {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RControl))
				return false;
		}
		if (shift) {
			if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RShift)))
				return false;
		}
		else {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RShift))
				return false;
		}
		if (alt) {
			if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LAlt) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RAlt)))
				return false;
		}
		else {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LAlt) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RAlt))
				return false;
		}
		return true;
	}
};

class Instrument {
	//wip
};

class Note {
	std::vector<float> volume;
	std::vector<float> pan;
	int width;
};

class Pattern {
private:
	std::vector<std::map<int, Note>> notes;
};

class Channel {
private:
	uint8_t cType;
public:
	std::string cName;
	int instrument = 1;
	std::vector<int> slots;
	std::map<int, Pattern> patterns;
	std::vector<Instrument> instruments;

	Channel(std::string name, uint8_t type)
	{
		cName = name;
		cType = type;
	};
};

#pragma endregion

#pragma region Instance Setup

//      __      __          __   __
// |_/ |_  \_/ |__) | |\ | |  \ (_
// | \ |__  |  |__) | | \| |__/ __)

Keybind kb_addCha(sf::Keyboard::Key::W, true);
Keybind kb_fullscreen(sf::Keyboard::Key::F11);
Keybind kb_debug(sf::Keyboard::Key::F3);
Keybind kb_cancel(sf::Keyboard::Key::Escape);

// setup
std::vector<Channel> channels;

#pragma endregion


#pragma region Utility functions
void setPopup(popups p) {
	popup = p;
	switch (p) {
	case popups::addCha:
		typing = true;
		inputString = "";
		inputPos = 0;
		inputWait = 1.0f;
		pressed = false;
	}
}

float approach(float a, float b, float c)
{
	if (debug)
		std::cout << "\t" << a;
	if (a < b)
		a += c;
	if (a > b)
		a -= c;
	if (debug) {
		std::cout << " && " << c << " = " << a << std::endl;
		if (a == b)
			std::cout << "[appr]\ta == " << b << std::endl;
	}
	return a;
}

// Sets some vars relatively to display size
void displaySetup(bool reset)
{
	// window reset to default or fullscreen size
	if (reset) {
		displayW = sf::VideoMode::getDesktopMode().width;
		displayH = sf::VideoMode::getDesktopMode().height;
		// display
		int w = displayW;
		int h = displayH;
		if (!fullscreen) {
			w = (int)((float)displayW * 0.8f);
			h = (int)((float)displayH * 0.8f);
			window.create(sf::VideoMode(w, h), title + " | " + version, sf::Style::Close);
		}
		else {
			window.create(sf::VideoMode(w, h), title + " | " + version, sf::Style::Fullscreen);
		}

		// icon
		sf::Image icon;
		icon.loadFromFile("images/icon64.png");
		window.setIcon(64, 64, icon.getPixelsPtr());
	}

	if (vsync) {
		window.setFramerateLimit(0);
		window.setVerticalSyncEnabled(true);
	}
	else
	{
		window.setVerticalSyncEnabled(false);
		window.setFramerateLimit(framerateLimit);
	}

	// var setup
	windowSize = window.getSize();
	if (reset)
		view.setSize((float)windowSize.x, (float)windowSize.y);
	boxSize = (int)((float)std::min(windowSize.x, windowSize.y) / 3.5f);
	out = (float)boxSize / 50.0f;
	scale = (float)out / 7.0f;
	pat = (int)std::ceil((float)boxSize / 8.0f);

	// rt
	if (reset) {
		int x, y, w, h;
		// channelScroll
		w = windowSize.x - boxSize;
		h = boxSize;
		channelScrollSize.x = w - out * 2.0f;
		channelScrollSize.y = h - out * 7.0f;
		rt_channelScroll.create(channelScrollSize.x, channelScrollSize.y);
	}

	drawReset = true;
}

// creates and returns an sfml color value using the current theme
sf::Color color(uint8_t index, uint8_t alpha)
{
	return sf::Color(theme[index][0], theme[index][1], theme[index][2], alpha);
}

bool button(sf::Vector2f p, sf::Vector2f& s, bool centered) {
	if (centered) {
		p.x -= s.x / 2;
		p.y -= s.y / 2;
	}

	sf::Vector2i pos = sf::Mouse::getPosition(window);

	if (pos.x > p.x) {
		if (pos.x < p.x + s.x) {
			if (pos.y > p.y) {
				if (pos.y < p.y + s.y) {
					return true;
				}
			}
		}
	}
	return false;
}

// clip text to max size
void textClip()
{
	inputString = inputString.substr(0, inputMax);
}

// setup clear color for render target clearing
sf::Color clear = color(c::bg, 255);
#pragma endregion

// add new channel
void addChannel() {
	popup = -1;
	channels.push_back(Channel(inputString, channelType::synth));
	channelsUpdated = true;
}

// text entered
void textEntered(char text) {
	if (!typing)
		return;
	if (!(isprint(text)))
		return;
	if (inputString.length() == inputMax)
		return;

	std::string part1 = inputString.substr(0, inputPos);
	std::string part2 = inputString.substr(inputPos, inputString.length());

	part1 += text;
	inputString = part1 + part2;
	inputPos++;
	inputWait = 0.0f;
	textClip();
}

// text backspace
void textBackspace()
{
	if (inputPos == 0)
		return;

	// with ctrl
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RControl))
	{
		std::string part1 = inputString.substr(0, std::max(0, inputPos));
		std::string part2 = inputString.substr(inputPos, inputString.length());
		if (part1[inputPos - 1] == ' ') {
			inputString = part1.substr(0, part1.length() - 1) + part2;
			inputPos = std::max(0, inputPos - 1);
			part1 = inputString.substr(0, std::max(0, inputPos));
			part2 = inputString.substr(inputPos, inputString.length());
		}
		while (part1.length() > 0 && part1[inputPos - 1] != ' ')
		{
			inputString = part1.substr(0, part1.length() - 1) + part2;
			inputPos = std::max(0, inputPos - 1);
			part1 = inputString.substr(0, std::max(0, inputPos));
			part2 = inputString.substr(inputPos, inputString.length());
		}
	}
	// normal
	else
	{
		std::string part1 = inputString.substr(0, std::max(0, inputPos - 1));
		std::string part2 = inputString.substr(inputPos, inputString.length());

		inputString = part1 + part2;
		inputPos = std::max(0, inputPos - 1);
	}

	inputWait = 0.0f;
}

// text delete
void textDelete()
{
	if (inputPos == inputString.length())
		return;

	std::string part1 = inputString.substr(0, std::max(0, inputPos));
	std::string part2 = inputString.substr(inputPos + 1);

	inputString = part1 + part2;
	inputWait = 0.0f;
}

// text paste
void textPaste()
{
	std::string clip = sf::Clipboard::getString();
	std::string part1 = inputString.substr(0, inputPos);
	std::string part2 = inputString.substr(inputPos, inputString.length());

	for (int i = 0; i < clip.length(); i++) {
		if (inputString.length() >= inputMax)
			break;
		part1 += clip[i];
	}

	inputString = part1 + part2;
	textClip();
	inputPos = std::min(inputString.length(), clip.length());
	inputWait = 0.0f;
}

// KEY PRESSED
void keyPressed(sf::Keyboard::Key key)
{
	// built in
	if (key == sf::Keyboard::Key::Backspace) {
		if (typing)
			textBackspace();
	}
	else if (key == sf::Keyboard::Key::Enter) {
		if (typing)
			addChannel();
	}
	else if (key == sf::Keyboard::Key::Delete) {
		if (typing)
			textDelete();
	}
	else if (key == sf::Keyboard::Key::V) {
		if (typing)
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RControl))
				textPaste();
	}
	else if (key == sf::Keyboard::Key::Left) {
		if (typing) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RControl)) {
				if (inputString[inputPos] == ' ')
					inputPos = std::max(0, inputPos - 1);
				while (inputString[inputPos] != ' ' && inputPos != 0)
					inputPos = std::max(0, inputPos - 1);
			}
			else
				inputPos = inputPos = std::max(0, inputPos - 1);;

			inputWait = 0.0f;
		}
	}
	else if (key == sf::Keyboard::Key::Right) {
		if (typing) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RControl)) {
				if (inputString[inputPos] == ' ')
					inputPos = std::min(inputPos + 1, (int)inputString.length());
				while (inputString[inputPos] != ' ' && inputPos != inputString.length())
					inputPos = std::min(inputPos + 1, (int)inputString.length());
			}
			else
				inputPos = std::min(inputPos + 1, (int)inputString.length());

			inputWait = 0.0f;
		}
	}
	else if (key == sf::Keyboard::Key::Home) {
		if (typing) {
			inputPos = 0;
			inputWait = 0.0f;
		}
	}
	else if (key == sf::Keyboard::Key::End) {
		if (typing) {
			inputPos = inputString.length();
			inputWait = 0.0f;
		}
	}
	// keybinds
	else if (kb_addCha.Check(key)) {
		if (popup == -1)
			setPopup(popups::addCha);
	}
	else if (kb_fullscreen.Check(key)) {
		fullscreen = !fullscreen;
		displaySetup(true);
	}
	else if (kb_debug.Check(key)) {
		debug = !debug;
	}
	else if (kb_cancel.Check(key)) {
		if (popup != 1) {
			popup = -1;
		}
	}
}

// KEY RELEASED
void keyReleased(sf::Keyboard::Key key)
{

}

// MOUSE PRESSED
void mousePressed(sf::Mouse::Button button)
{
	if (button == sf::Mouse::Button::Left) {
		pressed = true;

		switch (hover) {
		case hovers::addCh:
			setPopup(popups::addCha); break;
		case hovers::popupDone:
			if (popup == popups::addCha) {
				addChannel();
			}
		}
	}
}

// MOUSE RELEASED
void mouseReleased(sf::Mouse::Button button)
{
	if (button == sf::Mouse::Button::Left) {

	}
}

// handles SFML events
void eventHandler()
{
	while (window.pollEvent(event))
	{
		switch (event.type) {
		case sf::Event::EventType::Closed:
			window.close(); break;
		case sf::Event::KeyPressed:
			keyPressed(event.key.code); break;
		case sf::Event::KeyReleased:
			keyReleased(event.key.code); break;
		case sf::Event::MouseButtonPressed:
			mousePressed(event.mouseButton.button); break;
		case sf::Event::MouseButtonReleased:
			mouseReleased(event.mouseButton.button); break;
		case sf::Event::TextEntered:
			textEntered(event.text.unicode);
		}
	}
}

#pragma region main
// main
int main()
{
	// rpc
	g_Discord->Initialize();
	g_Discord->Update();

	// init
	displaySetup(true);
	if (!render.create(displayW, displayH))
		return 1;
	sf::Clock clock;
	int hoverLast = -1;
	int popupLast = -1;
	sf::Vector2i mouse;

	// shader
	sf::Shader shaderH;
	sf::Shader shaderV;
	if (!shaderH.loadFromFile("shaders/gaussianH.glsl", sf::Shader::Fragment))
		return 1;
	if (!shaderV.loadFromFile("shaders/gaussianV.glsl", sf::Shader::Fragment))
		return 1;
	shaderH.setUniform("u_texture", sf::Shader::CurrentTexture);
	shaderV.setUniform("u_texture", sf::Shader::CurrentTexture);
	shaderH.setUniform("resolution", (float)displayW);
	shaderV.setUniform("resolution", (float)displayH);

	// delta
	int v;
	float val;

	std::map<std::string, int> delta;
	delta["addH"] = 0;
	delta["channels"] = 100;
	delta["popup"] = 0;
	delta["p_addCha"] = 0;
	delta["popupH"] = 0;

	// textures
	sf::Texture t_logo256;
	sf::Texture t_plus;
	sf::Texture t_check;
	sf::Texture t_highlight1;
	sf::Texture t_highlight2;

	t_logo256.loadFromFile("images/logo256.png");
	t_plus.loadFromFile("images/plus.png");
	t_check.loadFromFile("images/check.png");
	t_check.setSmooth(true);
	t_highlight1.loadFromFile("images/highlight1.png");
	t_highlight2.loadFromFile("images/highlight2.png");

	// fonts
	sf::Font f_timeburner;

	f_timeburner.loadFromFile("fonts/timeburnernormal.ttf");

	// drawables
	// render
	sf::Sprite renderSprite;
	sf::Sprite channelScrollSprite;
	// vertex
	sf::Vertex dr_bar[] = { sf::Vertex(), sf::Vertex() };
	// bg
	sf::Sprite sp_bgb(t_logo256);
	sf::Sprite sp_bgf(t_logo256);
	// add box
	sf::RectangleShape dr_boxb;
	sf::RectangleShape dr_boxf;
	sf::Sprite bt_add(t_plus);
	sf::Sprite bt_addH1(t_highlight1);
	sf::Sprite bt_addH2(t_highlight2);
	sf::Text tx_add1("Add a new", f_timeburner);
	sf::Text tx_add2("channel", f_timeburner);
	// channels
	sf::RectangleShape dr_channelsb;
	sf::RectangleShape dr_channelsf;
	sf::RectangleShape dr_pattern;
	sf::Text tx_pattern;
	// popups
	sf::RectangleShape dr_popupbg;
	sf::RectangleShape dr_popupb;
	sf::RectangleShape dr_popupf;
	// add popup
	sf::RectangleShape dr_popupInput;
	sf::Text tx_popup("Channel name:", f_timeburner);
	sf::Text tx_popupInput("", f_timeburner);
	sf::Sprite bt_popup(t_check);
	sf::Sprite bt_popupH1(t_highlight1);
	sf::Sprite bt_popupH2(t_highlight2);

	// drawables' vars
	sf::Vector2f bt_add_p;
	sf::Vector2f bt_add_s;
	sf::Vector2f bt_popup_p;
	sf::Vector2f bt_popup_s;
	sf::Vector2u highlight_size = t_highlight1.getSize();
	sf::FloatRect textRect;
	int x, y, w, h;

	// loop
	while (window.isOpen())
	{
		// reset drawables
		if (drawReset) {
			// ########  ########  ######  ######## ######## 
			// ##     ## ##       ##    ## ##          ##    
			// ##     ## ##       ##       ##          ##    
			// ########  ######    ######  ######      ##    
			// ##   ##   ##             ## ##          ##    
			// ##    ##  ##       ##    ## ##          ##    
			// ##     ## ########  ######  ########    ##    

			sf::Vector2u v2u;

			// bg
			// sp_bgb
			w = t_logo256.getSize().x;
			x = (windowSize.x - w) / 2 + 1;
			y = (windowSize.y - w) / 2 + 1;
			sp_bgb.setPosition(x, y);
			sp_bgb.setColor(color(c::logo1, 255));
			// sp_bgf
			x = (windowSize.x - w) / 2;
			y = (windowSize.y - w) / 2;
			sp_bgf.setPosition(x, y);
			sp_bgf.setColor(color(c::logo2, 255));
			// add box
			// dr_boxb
			w = boxSize;
			h = boxSize;
			x = windowSize.x - w;
			y = windowSize.y - h;
			dr_boxb.setPosition(x, y);
			dr_boxb.setSize(sf::Vector2f(w, h));
			dr_boxb.setFillColor(color(c::outside, 255));
			dr_boxb.setOutlineColor(color(c::outline, 255));
			dr_boxb.setOutlineThickness(-1);
			// dr_boxf
			dr_boxf.setPosition(x + out, y + out);
			dr_boxf.setSize(sf::Vector2f(w - out * 2.0f, h - out * 7.0f));
			dr_boxf.setFillColor(color(c::inside, 255));
			dr_boxf.setOutlineColor(color(c::outline, 255));
			dr_boxf.setOutlineThickness(-1);
			// bt_add
			v2u = t_plus.getSize();
			bt_add.setColor(color(c::light2, 255));
			bt_add.setScale(scale, scale);
			bt_add.setOrigin((float)v2u.x / 2.0f * scale, (float)v2u.y / 2.0f * scale);
			bt_add.setPosition(std::floor((float)windowSize.x - boxSize / 2.0f), std::floor((float)windowSize.y - out * 3.5f));
			bt_add_p.x = (float)windowSize.x - (float)boxSize / 2.0f;
			bt_add_p.y = (float)windowSize.y - out * 3.5f;
			bt_add_s.x = (float)v2u.x * scale * 5.0f;
			bt_add_s.y = (float)v2u.x * scale * 2.0f;
			// bt_addH1
			bt_addH1.setColor(color(c::light1, 0));
			bt_addH1.setPosition(std::ceil((float)windowSize.x - (float)boxSize / 2.0f), std::floor((float)windowSize.y - out * 3.2f));
			bt_addH1.setOrigin(std::ceil((float)highlight_size.x / 2.0f), std::ceil((float)highlight_size.y / 2.0f));
			bt_addH1.setScale(sf::Vector2f(scale / 1.3f, scale / 1.3f));
			// bt_addH2
			bt_addH2.setColor(color(c::light1, 0));
			bt_addH2.setPosition(std::ceil((float)windowSize.x - (float)boxSize / 2.0f), std::floor((float)windowSize.y - out * 3.2f));
			bt_addH2.setOrigin(std::ceil((float)highlight_size.x / 2.0f), std::ceil((float)highlight_size.y / 2.0f));
			bt_addH2.setScale(sf::Vector2f(scale / 1.3f, scale / 1.3f));
			// tx_add1
			tx_add1.setCharacterSize((unsigned int)(scale * 40.0f));
			textRect = tx_add1.getLocalBounds();
			tx_add1.setOrigin(std::ceil(textRect.width / 2.0f), std::ceil(textRect.top + textRect.height / 2.0f));
			tx_add1.setFillColor(color(c::light1, 255));
			tx_add1.setPosition(std::ceil((float)windowSize.x - boxSize / 2.0f), std::ceil((float)windowSize.y - (float)boxSize / 2.0f - textRect.height));
			// tx_add1
			tx_add2.setCharacterSize((unsigned int)(scale * 40.0f));
			textRect = tx_add2.getLocalBounds();
			tx_add2.setOrigin(std::ceil(textRect.width / 2.0f), std::ceil(textRect.top + textRect.height / 2.0f));
			tx_add2.setFillColor(color(c::light1, 255));
			tx_add2.setPosition(std::ceil((float)windowSize.x - boxSize / 2.0f), std::ceil((float)windowSize.y - (float)boxSize / 2.0f + textRect.height / 1.75f));
			// channels
			// dr_channelsb
			w = windowSize.x - boxSize;
			h = boxSize;
			x = windowSize.x * delta["channels"];
			y = windowSize.y - boxSize;
			dr_channelsb.setPosition(x, y);
			dr_channelsb.setSize(sf::Vector2f(w, h));
			dr_channelsb.setFillColor(color(c::outside, 255));
			dr_channelsb.setOutlineColor(color(c::outline, 255));
			dr_channelsb.setOutlineThickness(-1);
			// dr_channelsf
			dr_channelsf.setPosition(x + out, y + out);
			dr_channelsf.setSize(sf::Vector2f(w - out * 2.0f, h - out * 7.0f));
			dr_channelsf.setFillColor(color(c::inside, 255));
			dr_channelsf.setOutlineColor(color(c::outline, 255));
			dr_channelsf.setOutlineThickness(-1);
			// dr_pattern
			dr_pattern.setSize(sf::Vector2f(pat, pat));
			dr_pattern.setFillColor(color(c::inside, 255));
			dr_pattern.setOutlineColor(color(c::outline, 255));
			dr_pattern.setOutlineThickness(-1);
			// popups
			// dr_popupbg
			w = windowSize.x;
			h = windowSize.y;
			dr_popupbg.setSize(sf::Vector2f(w, h));
			dr_popupbg.setFillColor(color(c::dark, 0));
			// dr_popupb
			w = windowSize.x / 3;
			h = windowSize.y / 5;
			x = (windowSize.x - w) / 2;
			y = (windowSize.y - h) / 2;
			dr_popupb.setPosition(x, y);
			dr_popupb.setSize(sf::Vector2f(w, h));
			dr_popupb.setFillColor(color(c::outside, 0));
			dr_popupb.setOutlineColor(color(c::outline, 0));
			dr_popupb.setOutlineThickness(-1);
			// dr_popupf
			dr_popupf.setPosition(x + out, y + out);
			dr_popupf.setSize(sf::Vector2f(w - out * 2.0f, h - out * 7.0f));
			dr_popupf.setFillColor(color(c::inside, 0));
			dr_popupf.setOutlineColor(color(c::outline, 0));
			dr_popupf.setOutlineThickness(-1);
			// dr_popupInput
			dr_popupInput.setPosition(x + out * 5.0f, y + h / 2.0f);
			dr_popupInput.setSize(sf::Vector2f(w - out * 10.0f, h / 5.45f));
			dr_popupInput.setFillColor(color(c::input, 0));
			dr_popupInput.setOutlineColor(color(c::outline, 0));
			dr_popupInput.setOutlineThickness(-1);
			// tx_popup
			tx_popup.setString("Channel name:");
			tx_popup.setCharacterSize((unsigned int)(scale * 40.0f));
			textRect = tx_popup.getLocalBounds();
			tx_popup.setOrigin(std::ceil(textRect.width / 2.0f), 0);
			tx_popup.setPosition(x + w / 2, y + h / 5);
			tx_add2.setFillColor(color(c::light1, 255));
			// tx_popupInput
			tx_popupInput.setCharacterSize((unsigned int)(scale * 30.0f));
			tx_popupInput.setString(inputString);
			textRect = tx_popupInput.getLocalBounds();
			tx_popupInput.setOrigin(std::ceil(textRect.width / 2.0f), 0);
			tx_popupInput.setPosition(x + w / 2, y + std::ceil(h / 1.8f));
			tx_popupInput.setFillColor(color(c::outside, 255));
			// dr_bar
			dr_bar[0].color = color(c::outline, 255);
			dr_bar[1].color = color(c::outline, 255);
			dr_bar[0].position.x = x + w / 2;
			dr_bar[1].position.x = x + w / 2;
			dr_bar[0].position.y = y + std::ceil(h / 1.8f);
			dr_bar[1].position.y = y + std::ceil(h / 1.8f) + out * 4.25f;
			// bt_popup
			v2u = t_check.getSize();
			bt_popup.setColor(color(c::light2, 255));
			bt_popup.setScale(scale * 0.55f, scale * 0.55f);
			bt_popup.setOrigin((float)v2u.x / 2.0f * scale * 1.25f, (float)v2u.y / 2.0f * scale * 0.55f);
			bt_popup.setPosition(std::floor((float)windowSize.x / 2.0f), std::ceil((float)y + (float)h - out * 4.15f));
			bt_popup_p.x = (float)windowSize.x / 2.0f;
			bt_popup_p.y = (float)y + (float)h - out * 4.15f;
			bt_popup_s.x = (float)v2u.x * scale * 0.55f * 5.0f;
			bt_popup_s.y = (float)v2u.x * scale * 0.55f * 2.0f;
			// bt_popupH1
			bt_popupH1.setColor(color(c::light1, 0));
			bt_popupH1.setPosition(std::floor((float)windowSize.x / 2.0f), std::floor(y + h - out * 3.2f));
			bt_popupH1.setOrigin(std::ceil((float)highlight_size.x / 2.0f), std::ceil((float)highlight_size.y / 2.0f));
			bt_popupH1.setScale(sf::Vector2f(scale / 1.3f, scale / 1.3f));
			// bt_popupH2
			bt_popupH2.setColor(color(c::light1, 0));
			bt_popupH2.setPosition(std::floor((float)windowSize.x / 2.0f), std::floor(y + h - out * 3.2f));
			bt_popupH2.setOrigin(std::ceil((float)highlight_size.x / 2.0f), std::ceil((float)highlight_size.y / 2.0f));
			bt_popupH2.setScale(sf::Vector2f(scale / 1.3f, scale / 1.3f));


			drawReset = false;

			// delta fix (easy performace fix fix ;) )
			delta["addH"] += 1;
			delta["popup"] += 1;

			if (debug)
				std::cout << "[drawables] reset" << std::endl;
		}

		// handle events
		eventHandler();

		// clear
		hoverLast = hover;
		hover = -1;
		mouse = sf::Mouse::getPosition(window);
		render.clear(clear);
		window.clear();
#pragma endregion
#pragma region Update
		// ##     ## ########  ########     ###    ######## ######## 
		// ##     ## ##     ## ##     ##   ## ##      ##    ##       
		// ##     ## ##     ## ##     ##  ##   ##     ##    ##       
		// ##     ## ########  ##     ## ##     ##    ##    ######   
		// ##     ## ##        ##     ## #########    ##    ##       
		// ##     ## ##        ##     ## ##     ##    ##    ##       
		// ######### ##        ########  ##     ##    ##    ######## 

		// input wait
		inputWait += 0.000001f * dt * 3.0f;

		// bt_add button
		if (popup == -1) {
			if (button(bt_add_p, bt_add_s, true)) {
				hover = hovers::addCh;
			}
		}
		else // close popup pressed
		{
			if (pressed) {
				sf::Vector2f p, s;
				p = dr_popupb.getPosition();
				s = dr_popupb.getSize();
				if (mouse.x < p.x || mouse.x > p.x + s.x || mouse.y < p.y || mouse.y > p.y + s.y) {
					popup = -1;
				}
			}
		}

		// bt_add highlight
		if (hoverLast != hover || (delta["addH"] > 0 && delta["addH"] < 100)) {
			if (hover == hovers::addCh)
				v = 100;
			else
				v = 0;

			delta["addH"] = approach(delta["addH"], v, std::ceil(std::abs((float)delta["addH"] - v) / 2.0f));
			val = (float)delta["addH"] / 100.0f;
			sf::Color cc = color(c::light1, 255 * val);
			bt_addH1.setScale(scale / 1.3f * val, scale / 1.3f);
			bt_addH2.setScale(scale / 1.3f * val, scale / 1.3f);
			bt_addH1.setColor(cc);
			bt_addH2.setColor(cc);
		}

		// channel box
		if (channelsUpdated) {
			if (channels.size() > 0)
				v = 0;
			else
				v = 100;

			delta["channels"] = approach(delta["channels"], v, std::ceil(std::abs((float)delta["channels"] - v) / 8.0f));
			val = (float)delta["channels"] / 100.0f;
			dr_channelsb.setPosition(windowSize.x * val, windowSize.y - boxSize);
			dr_channelsf.setPosition(windowSize.x * val + out, windowSize.y - boxSize + out);

			if (delta["channels"] == 0 || delta["channels"] == 100)
				channelsUpdated = false;
		}

		channelScrollApp.x = approach(channelScrollApp.x, channelScroll.x, std::abs(channelScrollApp.x - channelScroll.x) / 5.0f);
		channelScrollApp.y = approach(channelScrollApp.y, channelScroll.y, std::abs(channelScrollApp.y - channelScroll.y) / 5.0f);

		// popup
		if (popup == popups::addCha || popup == popups::renameChannel) {
			tx_popupInput.setString(inputString.substr(0, inputPos));
			textRect = tx_popupInput.getLocalBounds();
			int off = std::ceil(textRect.width);
			tx_popupInput.setString(inputString);
			textRect = tx_popupInput.getLocalBounds();
			tx_popupInput.setOrigin(std::ceil(textRect.width / 2.0f), 0);
			w = windowSize.x / 3 * val;
			h = windowSize.y / 5;
			x = (windowSize.x - w) / 2;
			y = (windowSize.y - h) / 2;
			dr_bar[0].position.x = x + w / 2 + off - textRect.width / 2.0f;
			dr_bar[1].position.x = x + w / 2 + off - textRect.width / 2.0f;

			if (button(bt_popup_p, bt_popup_s, true)) {
				hover = hovers::popupDone;
			}
		}

		// bt_popup highlight
		if (hoverLast != hover || (delta["popupH"] > 0 && delta["popupH"] < 100)) {
			if (hover == hovers::popupDone)
				v = 100;
			else
				v = 0;

			delta["popupH"] = approach(delta["popupH"], v, std::ceil(std::abs((float)delta["popupH"] - v) / 2.0f));
			val = (float)delta["popupH"] / 100.0f;
			sf::Color cc = color(c::light1, 255 * val);
			bt_popupH1.setScale(scale / 1.3f * val, scale / 1.3f);
			bt_popupH2.setScale(scale / 1.3f * val, scale / 1.3f);
			bt_popupH1.setColor(cc);
			bt_popupH2.setColor(cc);
		}

		if (popupLast != popup || (delta["popup"] != 0 && delta["popup"] != 100)) {
			if (popup == -1)
				v = 0;
			else
				v = 100;
			delta["popup"] = approach(delta["popup"], v, std::ceil(std::abs((float)delta["popup"] - v) / 2.5f));
			val = (float)delta["popup"] / 100.0f;

			// color
			sf::Color cc = dr_popupb.getFillColor();
			cc.a = val * 255.0f;
			dr_popupb.setFillColor(cc);
			cc = dr_popupf.getFillColor();
			cc.a = val * 255.0f;
			dr_popupf.setFillColor(cc);
			cc = dr_popupb.getOutlineColor();
			cc.a = val * 255.0f;
			dr_popupb.setOutlineColor(cc);
			dr_popupf.setOutlineColor(cc);
			cc = dr_popupbg.getFillColor();
			cc.a = val * 127.5f;	// 255 * 0.5 for bg alpha
			dr_popupbg.setFillColor(cc);

			// size
			switch (popup) {
			default: break;
			case popups::addCha:
				w = windowSize.x / 3 * val;
				h = windowSize.y / 5;
				x = (windowSize.x - w) / 2;
				y = (windowSize.y - h) / 2;
				dr_popupb.setPosition(x, y);
				dr_popupb.setSize(sf::Vector2f(w, h));
				dr_popupf.setPosition(x + out, y + out);
				dr_popupf.setSize(sf::Vector2f(w - out * 2.0f, h - out * 7.0f));
				dr_popupInput.setPosition(x + out * 5.0f, y + h / 2.0f + out);
				dr_popupInput.setSize(sf::Vector2f(w - out * 10.0f * val, h / 5.25f));
				dr_popupInput.setFillColor(color(c::input, 255 * val));
				dr_popupInput.setOutlineColor(color(c::outline, 255 * val));
			}
		}

		// button colors
		if (hoverLast != hover) {
			if (hover == hovers::addCh)
				bt_add.setColor(color(c::light1, 255));
			else
				bt_add.setColor(color(c::light2, 255));
			if (hover == hovers::popupDone)
				bt_popup.setColor(color(c::light1, 255));
			else
				bt_popup.setColor(color(c::light2, 255));
		}

#pragma endregion

		// ########  ########     ###    ##      ## 
		// ##     ## ##     ##   ## ##   ##  ##  ## 
		// ##     ## ##     ##  ##   ##  ##  ##  ## 
		// ##     ## ########  ##     ## ##  ##  ## 
		// ##     ## ##   ##   ######### ##  ##  ## 
		// ##     ## ##    ##  ##     ## ##  ##  ## 
		// ########  ##     ## ##     ##  ###  ###

		// bg
		render.draw(sp_bgb);
		render.draw(sp_bgf);

		// channels
		render.draw(dr_channelsb);
		render.draw(dr_channelsf);

		if (delta["channels"] == 0) {
			for (int i = 0; i < channels.size(); i++) {
				for (int j = 0; j < songLength; j++) {
					dr_pattern.setPosition((pat + out) * ((float)j + channelScrollApp.x), (pat + out) * ((float)i + channelScrollApp.x));
					rt_channelScroll.draw(dr_pattern);
				}
			}
			rt_channelScroll.display();

			channelScrollSprite.setTexture(rt_channelScroll.getTexture());
			render.draw(channelScrollSprite);
		}

		// add box
		render.draw(dr_boxb);
		render.draw(dr_boxf);
		render.draw(tx_add1);
		render.draw(tx_add2);
		render.draw(bt_add);
		render.draw(bt_addH1);
		render.draw(bt_addH2);
		render.display();
		renderSprite.setTexture(render.getTexture());

		if (popup == -1 || !shaders)
			window.draw(renderSprite);
		else
		{
			render.draw(renderSprite, &shaderH);
			renderSprite.setTexture(render.getTexture());
			window.draw(renderSprite, &shaderV);
		}

		// popups
		window.draw(dr_popupbg);
		window.draw(dr_popupb);
		window.draw(dr_popupf);
		if (popup == popups::addCha || popup == popups::renameChannel) {
			window.draw(dr_popupInput);
			window.draw(tx_popupInput);
			window.draw(tx_popup);
			window.draw(bt_popup);
			window.draw(bt_popupH1);
			window.draw(bt_popupH2);
			if (fmod(std::floor(inputWait), 2) == 0)
				window.draw(dr_bar, 2, sf::Lines);
		}

#pragma region main
		window.display();

		// set
		popupLast = popup;
		dt = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		pressed = false;
	}

	// end
	return 0;
}
#pragma endregion
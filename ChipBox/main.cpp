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

static std::string title = "ChipBox";
static std::string version = "devbuild 7.29.20";

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

sf::Int64 dt;	// delta time

sf::RenderWindow window(sf::VideoMode(300, 1), "Loading...");
sf::View view = window.getView();
sf::Event event;
sf::Vector2u windowSize = window.getSize();

sf::RenderTexture render;
sf::RenderTexture channelScroll;
sf::RenderTexture channelPanel;
sf::RenderTexture pianoKeys;
sf::RenderTexture pianoRoll;

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
};

enum popups
{
	addChannel,
	renameChannel
};

#pragma endregion

#pragma region Classes

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

#pragma region Class Setup

std::vector<Channel> channels;

#pragma endregion


#pragma region Utility functions
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
	pat = (int)std::ceil((float)boxSize / 7.0f);

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

// setup clear color for render target clearing
sf::Color clear = color(c::bg, 255);
#pragma endregion

void keyPressed(sf::Keyboard::Key key)
{
	switch (key) {
	default: break;
	case sf::Keyboard::Key::F3:
		debug = !debug;
		std::cout << "[debug] " << debug << std::endl; break;
	case sf::Keyboard::Key::F11:
		fullscreen = !fullscreen;
		displaySetup(true); break;
	case sf::Keyboard::Key::Escape:
		popup = -1; break;
	}
}

void keyReleased(sf::Keyboard::Key key)
{

}

void mousePressed(sf::Mouse::Button button)
{
	if (button == sf::Mouse::Button::Left) {
		pressed = true;

		switch (hover) {
		case hovers::addCh:
			popup = popups::addChannel; pressed = false; break;
		}
	}
}

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
		case sf::Event::Resized:
			view.setSize((float)(event.size.width), (float)(event.size.height));
			window.setSize(sf::Vector2u(event.size.width, event.size.height));
			displaySetup(false); break;
		case sf::Event::MouseButtonPressed:
			mousePressed(event.mouseButton.button); break;
		case sf::Event::MouseButtonReleased:
			mouseReleased(event.mouseButton.button); break;
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

	// textures
	sf::Texture t_logo256;
	sf::Texture t_plus;
	sf::Texture t_highlight1;
	sf::Texture t_highlight2;

	t_logo256.loadFromFile("images/logo256.png");
	t_plus.loadFromFile("images/plus.png");
	t_highlight1.loadFromFile("images/highlight1.png");
	t_highlight2.loadFromFile("images/highlight2.png");

	// fonts
	sf::Font f_timeburner;

	f_timeburner.loadFromFile("fonts/timeburnernormal.ttf");

	// drawables
	// render
	sf::Sprite renderSprite;
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
	// popups
	sf::RectangleShape dr_popupbg;
	sf::RectangleShape dr_popupb;
	sf::RectangleShape dr_popupf;
	sf::RectangleShape dr_popupInput;
	sf::Text tx_popup("Channel name:", f_timeburner);
	sf::Text tx_popupInput("", f_timeburner);

	// drawables' vars
	sf::Vector2f bt_add_p;
	sf::Vector2f bt_add_s;
	sf::Vector2u highlight_size = t_highlight1.getSize();

	// loop
	while (window.isOpen())
	{
		// reset drawables
		if (drawReset) {
			sf::Vector2u v2u;
			int x, y, w, h;

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
			bt_add.setColor(color(c::inside, 255));
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
			sf::FloatRect textRect = tx_add1.getLocalBounds();
			tx_add1.setOrigin(std::ceil(textRect.left + textRect.width / 2.0f), std::ceil(textRect.top + textRect.height / 2.0f));
			tx_add1.setFillColor(color(c::light1, 255));
			tx_add1.setPosition(std::ceil((float)windowSize.x - boxSize / 2.0f), std::ceil((float)windowSize.y - (float)boxSize / 2.0f - textRect.height));
			// tx_add1
			tx_add2.setCharacterSize((unsigned int)(scale * 40.0f));
			textRect = tx_add2.getLocalBounds();
			tx_add2.setOrigin(std::ceil(textRect.left + textRect.width / 2.0f), std::ceil(textRect.top + textRect.height / 2.0f));
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
			// tx_popup

			drawReset = false;

			// delta fix (performace fix fix ;) )
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

		// bt_add color
		if (hoverLast != hover) {
			if (hover == hovers::addCh)
				bt_add.setColor(color(c::light1, 255));
			else
				bt_add.setColor(color(c::inside, 255));
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

			delta["channels"] = approach(delta["channels"], v, std::ceil(std::abs((float)delta["channels"] - v) / 6.0f));
			val = (float)delta["channels"] / 100.0f;
			dr_channelsb.setPosition(windowSize.x * val, windowSize.y - boxSize);
			dr_channelsf.setPosition(windowSize.x * val + out, windowSize.y - boxSize + out);

			if (delta["channels"] == 0 || delta["channels"] == 100)
				channelsUpdated = false;
		}

		// popup
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
			case popups::addChannel:
				int x, y, w, h;
				w = windowSize.x / 3 * val;
				h = windowSize.y / 5;
				x = (windowSize.x - w) / 2;
				y = (windowSize.y - h) / 2;
				dr_popupb.setPosition(x, y);
				dr_popupb.setSize(sf::Vector2f(w, h));
				dr_popupf.setPosition(x + out, y + out);
				dr_popupf.setSize(sf::Vector2f(w - out * 2.0f, h - out * 7.0f));
			}
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
		window.draw(dr_popupInput);
		window.draw(tx_popupInput);

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
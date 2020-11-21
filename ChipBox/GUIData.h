#pragma once
#include <cstdint>

// window
sf::Vector2u res(300, 1);
sf::String windowName = "ChipBox | devbuild 11.8.20";

sf::RenderWindow mainWindow(sf::VideoMode(res.x, res.y), "Loading...");

// icon
sf::Image icon;

// parameters
bool fullscreen = false;
bool vsync = true;
unsigned int framerate = 60;

// delta time
float dt = 1.0f;

// approach val
std::map<const char *, float> val;
float v;
float tempv;

// mouse
sf::Vector2i mouse;

// clear color
sf::Color clearColor;

// interface scaling
sf::Vector2u display;

int boxSize, pat, patDist;
float out, scale;

// shaders
bool shaders = true;
sf::Shader shaderH;
sf::Shader shaderV;

// popups
enum class popups
{
	none,
	addChannel,
	renameChannel,
};

popups popup = popups::none;

// usage : theme[c::bg] NOT theme[1]
// for code readability
enum class c
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
	clearWhite,
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
	{255, 255, 255},	// clearWhite
};

enum class hovers
{
	none,
	addBox,
};

hovers hover = hovers::none;
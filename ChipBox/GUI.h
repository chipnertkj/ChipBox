#pragma once
#include "SFML/Graphics.hpp"
#include "GUIClasses.h"
#include "ProjectClasses.h"

namespace gui {
	// resources
	sf::Texture tx_plus;
	sf::Texture tx_highlight1;
	sf::Texture tx_highlight2;
	sf::Texture tx_logo32;

	// objects
	Render mainRender;
	Project project;

	// drawables
	Menu d_menu;
	Container d_addBox;
	Button b_addBox;
	Container d_channelBox;
	Scroll d_channels;
	Container d_popup;
	sf::Vertex popupBG[4];

	// interface scaling
	// this is where ui elements are actually described
	void setup() {
		// setup interface vars
		boxSize = (int)((float)std::min(res.x, res.y) / 3.5f);
		out = (float)boxSize / 50.0f;
		scale = out / 7.0f;
		pat = (int)std::ceil((float)boxSize / 8.0f);
		patDist = (int)(out * 0.75f);

		// approach values
		val["channelBoxPos"] = 0.0f;
		val["channelsAlpha"] = 0.0f;
		val["addBoxAlpha"] = 0.0f;
		val["popupAlpha"] = 0.0f;

		// setup drawables
		// clear color
		clearColor = color(c::bg, 255);
		// d_menu
		d_menu.initPos();
		d_menu.initTheme();
		// d_addBox
		d_addBox.set((float)(res.x - boxSize), (float)(res.y - boxSize), (float)boxSize, (float)boxSize);
		d_addBox.initPos();
		d_addBox.initTheme();
		b_addBox.hov = hovers::addBox;
		// b_addBox
		b_addBox.x = std::floor(d_addBox.x + d_addBox.w / 2);
		b_addBox.y = std::floor(d_addBox.y + d_addBox.h - out * (d_addBox.spacing + d_addBox.bottom / 2.0f) + out / 3.0f);
		b_addBox.load(tx_plus, tx_highlight1, tx_highlight2);
		b_addBox.initPos();
		// d_channelBox
		d_channelBox.set(0.0f, (float)(res.y - boxSize), (float)(res.x - boxSize), (float)boxSize);
		d_channelBox.initPos();
		d_channelBox.initTheme();
		// d_channels
		d_channels.copy(d_channelBox);
		// popupBG
		for (int i = 0; i < 4; i++)
			popupBG[i].color = color(c::dark, 0);
		popupBG[0].position.x = 0;				popupBG[0].position.y = 0;
		popupBG[1].position.x = (float)res.x;	popupBG[1].position.y = 0;
		popupBG[2].position.x = (float)res.x;	popupBG[2].position.y = (float)res.y;
		popupBG[3].position.x = 0;				popupBG[3].position.y = (float)res.y;

	}

	// load resources
	bool load() {
		// shaders
		if (!shaderH.loadFromFile("shaders/gaussianH.glsl", sf::Shader::Fragment))
			return false;
		if (!shaderV.loadFromFile("shaders/gaussianV.glsl", sf::Shader::Fragment))
			return false;

		// texture uniforms
		shaderH.setUniform("u_texture", sf::Shader::CurrentTexture);
		shaderV.setUniform("u_texture", sf::Shader::CurrentTexture);

		if (!tx_logo32.loadFromFile("images/logo32.png"))
			return false;
		tx_logo32.setSmooth(true);
		d_menu.load(tx_logo32);

		if (!icon.loadFromFile("images/icon64.png"))
			return false;
		if (!tx_plus.loadFromFile("images/plus.png"))
			return false;
		if (!tx_highlight1.loadFromFile("images/highlight1.png"))
			return false;
		if (!tx_highlight2.loadFromFile("images/highlight2.png"))
			return false;

		return true;
	}

	// initialize the render targets
	void init(bool recreate) {
		// display
		sf::VideoMode vm = sf::VideoMode::getDesktopMode();
		display.x = vm.width;
		display.y = vm.height;

		// window
		if (recreate) {
			if (fullscreen) {
				vm.height++;
				mainWindow.create(vm, windowName, sf::Style::None);
			}
			else {
				vm.width = (unsigned int)(vm.width * 0.8f);
				vm.height = (unsigned int)(vm.height * 0.8f);
				mainWindow.create(vm, windowName, sf::Style::Close);
			}
			// set icon
			mainWindow.setIcon(64, 64, icon.getPixelsPtr());

			// (re)create renders
			mainRender.create(vm.width, vm.height);

			// shader uniforms
			shaderH.setUniform("resolution", (float)vm.width);
			shaderV.setUniform("resolution", (float)vm.height);
		}

		res.x = vm.width;
		res.y = vm.height;

		// framerate
		if (vsync) {
			mainWindow.setFramerateLimit(0);
			mainWindow.setVerticalSyncEnabled(true);
		}
		else {
			mainWindow.setVerticalSyncEnabled(false);
			mainWindow.setFramerateLimit(framerate);
		}

		// interface setup
		setup();
	}

	// main tick
	void update() {
		hover = hovers::none;
		mouse = sf::Mouse::getPosition(mainWindow);

		// d_addBox and its content
		if (popup == popups::none)
			b_addBox.check();
		setV(hover == b_addBox.hov);
		smooth("addBoxAlpha", v, 2.0f);
		b_addBox.updateAppr();

		// d_channelBox and d_channels position
		setV(project.channelCount > 0);
		smooth("channelBoxPos", v, 6.0f);
		tempv = lerp((float)(res.x - boxSize) + out, 0.0f, val["channelBoxPos"]);
		d_channelBox.x = tempv;
		d_channelBox.initPos();
		d_channels.follow(d_channelBox);
		// d_channelBox scroll alpha
		setV(val["channelBoxPos"] > 0.99f);
		smooth("channelsAlpha", v, 3.0f);
		tempv = val["channelsAlpha"] * 255.0f;
		d_channels.render.sprite.setColor(color(c::clearWhite, (uint8_t)tempv));

		// popups
		setV(popup != popups::none);
		smooth("popupAlpha", v, 2.25f);
		if (val["popupAlpha"] > 0.0f) {
			tempv = val["popupAlpha"] * 100.0f;
			for (int i = 0; i < 4; i++)
				popupBG[i].color = color(c::dark, (uint8_t)tempv);
		}

	}

	// main application renderer
	void render() {
		// clear
		mainWindow.clear(clearColor);
		mainRender.texture.clear(clearColor);
		d_channels.render.texture.clear();

		// draw drawables
		d_channelBox.draw(mainRender.texture);
		d_channels.draw(mainRender.texture);
		d_addBox.draw(mainRender.texture);
		b_addBox.draw(mainRender.texture);

		// render main render to window
		mainRender.texture.display();
		mainRender.sprite.setTexture(mainRender.texture.getTexture());

		// no popups or shaders off == render mainrender to window normally
		if (popup == popups::none || !shaders) {
			mainWindow.draw(mainRender.sprite);
		}
		else {	// else render with a two-pass blur shader
			mainRender.texture.draw(mainRender.sprite, &shaderH);
			mainRender.texture.display();
			mainRender.sprite.setTexture(mainRender.texture.getTexture());
			mainWindow.draw(mainRender.sprite, &shaderV);
		}

		// draw on top of the main render (popups, menu bar)
		mainWindow.draw(popupBG, 4, sf::Quads);
		d_menu.draw(mainWindow);

		// display window
		mainWindow.display();
	}
}
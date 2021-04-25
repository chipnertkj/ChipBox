#include "GUI.h" // interface
#include "App.h"
#include "GUIData.h"
#include "GUIUtility.h"
#include "Input.h"

#include <iostream>
#include <algorithm>

namespace gui {
	// gui resizing
	bool resizeStarted = false;

	// widgets
	Widget wd_addbox;
	Widget wd_channels;
	Channels channels;
}

namespace gui {
	// one-time setup
	void setup() {
		/// wd_addbox
		wd_addbox.addButton(new ButtonBig);
		wd_addbox.buttons[0]->setTextures(app::tx_plus, app::tx_buttonHighlightBig);
		wd_addbox.handlesSetup(app::tx_dragHandle);
		/// wd_channels
		wd_channels.handlesSetup(app::tx_dragHandle);
		wd_channels.createScrollPanel();
		// channels
		channels.panel = wd_channels.scrollPanel;
	}

	

	// GUI scaling
	void init(bool resize, bool recreate) {
		std::cout << "Adjusting the GUI.\n";

		/// cs
		cs.boxW = std::max(cs.boxDefault, std::min((int)app::res.width - cs.boxDefault, cs.boxW));
		cs.boxH = std::max(cs.boxDefault, std::min((int)app::res.height - cs.boxDefault, cs.boxH));

		/// wd_addbox
		wd_addbox.set(
			(float)app::res.width - cs.boxW, (float)app::res.height - cs.boxH,
			(float)cs.boxW, (float)cs.boxH
		);

		wd_addbox.buttons[0]->set(
			wd_addbox.pos.x + wd_addbox.size.x / 2, wd_addbox.pos.y + wd_addbox.size.y - cs.space * ((wd_addbox.bottom + 0.5f) / 2.f),
			cs.boxDefault / 3.f, cs.space * wd_addbox.bottom * 0.8f
		);
		// buttons
		wd_addbox.buttons[0]->getSprite().setColor(color(c::inside));
		wd_addbox.buttons[0]->getHighlight().setColor(color(c::light1));
		wd_addbox.buttons[0]->setScale(cs.scale, cs.scale);
		wd_addbox.buttons[0]->round();
		// finish
		wd_addbox.checkLeft = true;
		wd_addbox.checkTop = true;
		wd_addbox.round();
		wd_addbox.update();

		/// wd_channels
		wd_channels.set(
			0.f, (float)app::res.height - ((app::project.channels.size() == 0) ? 0 : cs.boxH),
			(float)app::res.width - cs.boxW, (float)cs.boxH
		);
		wd_channels.checkTop = true;
		wd_channels.round();
		wd_channels.update();
		// channels scroll
		wd_channels.scrollPanel->set(wd_channels.getInsidePos(), wd_channels.getInsideSize());
		wd_channels.scrollPanel->update();
		if (recreate)
			wd_channels.scrollPanel->create();
		channels.update();
	}

	// GUI loop
	void update() {
		sf::Vector2i mouse = sf::Mouse::getPosition(app::mainWindow);

		/// gui resizing
		wd_addbox.check(mouse.x, mouse.y);
		wd_channels.check(mouse.x, mouse.y);

		/// wd_addbox
		// buttons
		if (wd_addbox.buttons[0]->check(mouse.x, mouse.y))
			if (input::onClick())
				app::project.addChannel("Channel " + std::to_string(app::project.channels.size()));
		wd_addbox.buttons[0]->animate();

		/// wd_channels
		// approach
		float v = float((app::project.channels.size() == 0U) ? app::res.height : (app::res.height - cs.boxH));
		wd_channels.pos.y = std::floor(approach(wd_channels.pos.y, v, std::abs(wd_channels.pos.y - v)/5.f)*100.f)/100.f;
		wd_channels.scrollPanel->set(wd_channels.getInsidePos(), wd_channels.getInsideSize());
		wd_channels.update();
		// scroll
		wd_channels.scrollPanel->check(mouse.x, mouse.y);
		wd_channels.scrollPanel->smooth();
	}

	// GUI rendering
	void render() {
		// clear
		app::mainWindow.clear(color(c::bg));
		app::mainRender.clear(color(c::bg));

		// wd_channels
		wd_channels.scrollPanel->clear(color(c::inside));
		channels.draw(wd_channels.scrollPanel->getTexture());
		wd_channels.scrollPanel->display();
		wd_channels.draw(app::mainRender.texture);

		// wd_addbox
		wd_addbox.draw(app::mainRender.texture);

		// renderer (shaders)
		app::mainRender.display();
		if (true) { // normal
			app::mainWindow.draw(app::mainRender.sprite);
		}
		else { // two pass blur
			app::mainRender.texture.draw(app::mainRender.sprite, &app::shaderH);
			app::mainRender.display();
			app::mainWindow.draw(app::mainRender.sprite, &app::shaderV);
		}

		app::mainWindow.display();
	}
}
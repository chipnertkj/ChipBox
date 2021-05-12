#include "GUI.h" // interface
#include "App.h"
#include "GUIData.h"
#include "GUIUtility.h"
#include "Input.h"

#include <algorithm>

namespace gui {
	// gui resizing
	bool cursorDragStarted = false;
	bool guiScalingStarted = false;

	// channels display
	Channels channels;

	// widgets
	Widget wd_addbox;
	Widget wd_channels;
}

namespace gui {
	// one-time setup
	void setup() {
		/// wd_addbox
		wd_addbox.addButton(new ButtonBig);
		wd_addbox.handlesSetup();
		wd_addbox.buttons[0]->setTextures(app::tx_plus, app::tx_buttonHighlightBig);
		/// wd_channels
		wd_channels.handlesSetup();
		wd_channels.createScrollPanel();
		// channels
		channels.scrollPanel = wd_channels.scrollPanel;
		channels.scrollPanel->scrollScale = &channels.scale;
	}

	// GUI scaling
	void init(bool resize, bool recreate) {
		app::cslog("GUI", "Adjusting widgets to scaling.");

		/// cs
		cs.boxW = std::max(cs.boxBoundsScaled, std::min((int)app::res.width - cs.boxBoundsScaled, cs.boxW));
		cs.boxH = std::max(cs.boxBoundsScaled, std::min((int)app::res.height - cs.boxBoundsScaled, cs.boxH));

		/// wd_addbox
		wd_addbox.set(
			(float)app::res.width - cs.boxW, (float)app::res.height - cs.boxH,
			(float)cs.boxW, (float)cs.boxH
		);
		wd_addbox.buttons[0]->set(
			wd_addbox.pos.x + wd_addbox.size.x / 2, wd_addbox.pos.y + wd_addbox.size.y - cs.space * ((wd_addbox.bottom + 0.5f) / 2.f),
			cs.boxBoundsScaled / 3.f, cs.space * wd_addbox.bottom * 0.8f
		);
		// handles
		wd_addbox.setHandlesScale(cs.scale, cs.scale);
		// buttons
		wd_addbox.buttons[0]->setScale(cs.scale, cs.scale);
		wd_addbox.buttons[0]->round();
		// finish
		wd_addbox.checkLeft = true;
		wd_addbox.checkTop = true;
		wd_addbox.round();
		wd_addbox.update();

		/// wd_channels
		wd_channels.set(
			0.f, (float)app::res.height - ((proj::project.channels.size() == 0) ? 0 : cs.boxH),
			(float)app::res.width - cs.boxW, (float)cs.boxH
		);
		wd_channels.checkTop = true;
		wd_channels.round();
		wd_channels.update();
		/// channels scroll
		wd_channels.scrollPanel->set(wd_channels.getInsidePos(), wd_channels.getInsideSize());
		wd_channels.scrollPanel->update();
		if (recreate)
			wd_channels.scrollPanel->create();

		/// channels
		if (recreate)
			channels.recalculate();
	}

	// GUI loop
	void update() {
		// mouse pos
		sf::Vector2i mouse = sf::Mouse::getPosition(app::mainWindow);

		/// gui resizing
		wd_addbox.check(mouse.x, mouse.y);
		wd_channels.check(mouse.x, mouse.y);

		/// wd_addbox
		// buttons
		if (wd_addbox.buttons[0]->check(mouse.x, mouse.y))
			if (input::onClick()) {
				proj::project.addChannel("Channel " + std::to_string(proj::project.channels.size()));
			}
		wd_addbox.buttons[0]->animate();

		/// wd_channels
		// approach
		float v = float((proj::project.channels.size() == 0U) ? app::res.height : (app::res.height - cs.boxH));
		wd_channels.pos.y = std::floor(approach(wd_channels.pos.y, v, std::abs(wd_channels.pos.y - v)/5.f)*100.f)/100.f;
		wd_channels.scrollPanel->set(wd_channels.getInsidePos(), wd_channels.getInsideSize());
		wd_channels.update();
		// scroll
		wd_channels.scrollPanel->check(mouse.x, mouse.y);
		wd_channels.scrollPanel->clamp();
		wd_channels.scrollPanel->smooth();
		// channels
		channels.update();
		channels.refreshText(); // TODO: (OPTIMIZATION, 26%) use only when necessary
	}

	// GUI rendering
	void render() {
		/// clear
		app::mainWindow.clear(th.background);
		app::mainRender.clear(th.background);

		/// wd_channels
		// clear
		wd_channels.scrollPanel->clear(th.boxInside);
		// channels
		channels.draw(wd_channels.scrollPanel->getTexture());
		// draw
		wd_channels.scrollPanel->display();
		wd_channels.draw(app::mainRender.texture);

		/// wd_addbox
		wd_addbox.draw(app::mainRender.texture);

		/// renderer (shaders)
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
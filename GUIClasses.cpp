#include "GUIClasses.h"	// interface
#include "GUI.h"
#include "GUIData.h"
#include "GUIUtility.h"
#include "App.h"
#include "Input.h"

#include <iostream>
#include <algorithm>
#include <cmath>
#include <stdlib.h>

namespace gui {
	//---------------------------------------------------------
	// Empty
	void Empty::setPos(float x, float y) {
		pos.x = x; pos.y = y;
	}
	void Empty::setPos(sf::Vector2f p) {
		pos = p;
	}
	void Empty::setSize(float w, float h) {
		size.x = w; size.y = h;
	}
	void Empty::setSize(sf::Vector2f s) {
		size = s;
	}

	void Empty::set(float x, float y, float w, float h) {
		pos.x = x; pos.y = y; size.x = w; size.y = h;
	}
	void Empty::set(sf::Vector2f p, sf::Vector2f s) {
		pos = p; size = s;
	}
	void Empty::round() {
		pos.x = std::round(pos.x); pos.y = std::round(pos.y);
		size.x = std::round(size.x); size.y = std::round(size.y);
	}
	void Empty::draw(sf::RenderTarget& target) {}
	void Empty::update() {}

	//---------------------------------------------------------
	// Rectangle
	void Rectangle::update() {
		vert[0].position.x = pos.x;				vert[0].position.y = pos.y;
		vert[1].position.x = pos.x + size.x;	vert[1].position.y = pos.y;
		vert[2].position.x = pos.x + size.x;	vert[2].position.y = pos.y + size.y;
		vert[3].position.x = pos.x;				vert[3].position.y = pos.y + size.y;
	}
	void Rectangle::setColor(sf::Color col) {
		for (int i = 0; i < vertCount; i++)
			vert[i].color = col;
	}
	void Rectangle::draw(sf::RenderTarget& target) {
		target.draw(vert, vertCount, sf::Quads);
	}

	//---------------------------------------------------------
	// RectangleOut
	void RectangleOut::update() {
		vert[0].position.x = pos.x-1;			vert[0].position.y = pos.y;
		vert[1].position.x = pos.x + size.x;	vert[1].position.y = pos.y;
		vert[2].position.x = pos.x + size.x;	vert[2].position.y = pos.y + size.y;
		vert[3].position.x = pos.x;				vert[3].position.y = pos.y + size.y;
		vert[4].position.x = pos.x;				vert[4].position.y = pos.y;
	}
	void RectangleOut::setColor(sf::Color col) {
		for (int i = 0; i < vertCount; i++)
			vert[i].color = col;
	}
	void RectangleOut::draw(sf::RenderTarget& target) {
		target.draw(vert, vertCount, sf::LineStrip);
	}

	//---------------------------------------------------------
	// Button
	sf::Sprite& Button::getSprite() {
		return sprite;
	}
	sf::Sprite& Button::getHighlight() {
		return highlight;
	}
	void Button::setTextures(sf::Texture& tx1, sf::Texture& tx2) {
		getSprite().setTexture(tx1);
		getSprite().setOrigin((sf::Vector2f)tx1.getSize() / 2.f);
		getHighlight().setTexture(tx2);
		getHighlight().setOrigin((sf::Vector2f)tx2.getSize()/2.f);
	}
	void Button::setScale(float sx, float sy) {}
	bool Button::check(int mx, int my) {
		checked = false;
		mx += (int)size.x / 2;
		my += (int)size.y / 2;
		if (hover((float)mx, (float)my, pos.x, pos.y, size.x, size.y)) {
			checked = true;
			app::mainWindow.setMouseCursor(app::cursor_hand);
			app::cursorChanged = true;
		}
		return checked;
	}
	void Button::animate() {}
	void Button::update() {
		getSprite().setPosition(pos);
		getHighlight().setPosition(pos);
	}
	void Button::draw(sf::RenderTarget& target) {
		target.draw(getSprite());
		target.draw(getHighlight());
	}

	//---------------------------------------------------------
	// ButtonBig
	void ButtonBig::setScale(float sx, float sy) {
		getSprite().setScale(sx*0.7f, sy*0.7f);
		getHighlight().setScale(0.f, sy*0.5f);
		defScale = { sx, sy };
	}
	void ButtonBig::animate() {
		getSprite().setColor(color(checked ? c::light1 : c::inside));
		sf::Vector2f sc = getHighlight().getScale();
		float v = (checked ? (defScale.x * 0.5f) : 0.f);
		getHighlight().setScale(
			approach(sc.x, v, std::abs(sc.x - v) / 2.f),
			sc.y
		);
		sc = getHighlight().getScale();
		getHighlight().setColor(color(c::light1, uint8_t(std::min(1.f, (float)pow(sc.x/(defScale.x*0.5f), 0.4f))*255U)));
	}

	//---------------------------------------------------------
	// Widget
	void Widget::update() {
		// bg
		bg.set(pos, size);
		bg.setColor(color(c::outside));
		bg.update();
		// bg outline
		bgOut.set(pos, size);
		bgOut.setColor(color(c::outline));
		bgOut.update();
		// front
		front.set(
			pos.x + cs.space, pos.y + cs.space,
			size.x - cs.space * 2, size.y - bottom * cs.space - cs.space
		);
		front.setColor(color(c::inside));
		front.update();
		// front outline
		frontOut.set(front.pos, front.size);
		frontOut.setColor(color(c::outline));
		frontOut.update();
		// scroll panel
		if (scroll)
			scrollPanel->update();
		// buttons
		for (auto& button : buttons)
			button->update();
		// drag 
		sprDragL.setPosition(pos.x + cs.space / 2 - 1, pos.y + (int)size.y / 2);
		sprDragT.setPosition(pos.x + (int)size.x / 2, pos.y + cs.space / 2 - 1);
		sprDragT.setRotation(90);
	}
	void Widget::draw(sf::RenderTarget& target) {
		bg.draw(target);
		front.draw(target);
		bgOut.draw(target);
		frontOut.draw(target);

		if (checkLeft)
			target.draw(sprDragL);
		if (checkTop)
			target.draw(sprDragT);

		if (scroll)
			scrollPanel->draw(target);

		for (auto& button : buttons)
			button->draw(target);
	}
	void Widget::addButton(Button* button) {
		buttons.push_back(button);
	}
	void Widget::handlesSetup(sf::Texture& tx) {
		sprDragL.setTexture(tx);
		sprDragL.setOrigin(sf::Vector2f(tx.getSize() / 2U));
		sprDragT.setTexture(tx);
		sprDragT.setOrigin(sf::Vector2f(tx.getSize() / 2U));
		sprDragL.setColor(color(c::outline));
		sprDragT.setColor(color(c::outline));
	}
	void Widget::createScrollPanel() {
		scrollPanel = new ScrollPanel;
		scroll = true;
	}
	sf::Vector2f Widget::getInsidePos() {
		return front.pos;
	}
	sf::Vector2f Widget::getInsideSize() {
		return front.size;
	}
	void Widget::check(int mx, int my) {
		bool passLeft = false;
		bool passTop = false;

		if (!resizeStarted) {
			if (checkLeft)
				if (hover(mx, my, pos.x, pos.y, cs.space, size.y)) {
					mousePosStart = sf::Mouse::getPosition(app::mainWindow);
					passLeft = true;
					if (sf::Mouse::isButtonPressed(sf::Mouse::Middle)) {
						if (cs.boxW != cs.boxDefault)
							cs.boxW = cs.boxDefault;
						else
							cs.boxW = app::res.width - cs.boxDefault;
						init(false, true);
					}
					else if (input::clicked) {
						dragLeft = true;
						mousePosLast = sf::Mouse::getPosition(app::mainWindow);
					}
				}
			if (checkTop)
				if (hover(mx, my, pos.x, pos.y, size.x, cs.space)) {
					mousePosStart = sf::Mouse::getPosition(app::mainWindow);
					passTop = true;
					if (sf::Mouse::isButtonPressed(sf::Mouse::Middle)) {
						if (cs.boxH != cs.boxDefault)
							cs.boxH = cs.boxDefault;
						else
							cs.boxH = app::res.width - cs.boxDefault;
						init(false, true);
					}
					else if (input::clicked) {
						dragTop = true;
						mousePosLast = sf::Mouse::getPosition(app::mainWindow);
					}
				}
		}

		if (dragLeft || dragTop) {
			input::clicked = false;
			resizeStarted = true;
		}
		else
			if (!resizeStarted) {
				sprDragL.setColor(color(c::outline));
				sprDragT.setColor(color(c::outline));
			}

		if (!app::cursorChanged) {
			if (passLeft && passTop) {
				app::mainWindow.setMouseCursor(app::cursor_tleftbright);
				sprDragL.setColor(color(c::light1));
				sprDragT.setColor(color(c::light1));
				app::cursorChanged = true;
			}
			else if (passLeft) {
				app::mainWindow.setMouseCursor(app::cursor_sizeH);
				sprDragL.setColor(color(c::light1));
				app::cursorChanged = true;
			}
			else if (passTop) {
				app::mainWindow.setMouseCursor(app::cursor_sizeV);
				sprDragT.setColor(color(c::light1));
				app::cursorChanged = true;
			}
		}

		sf::Vector2i mpos = sf::Mouse::getPosition(app::mainWindow);

		if (mousePosLast != mpos || !sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			if (dragLeft && dragTop) {
				cs.boxW = std::max(cs.boxDefault, std::min((int)app::res.width - cs.boxDefault, cs.boxW + mousePosLast.x - mpos.x));
				cs.boxH = std::max(cs.boxDefault, std::min((int)app::res.height - cs.boxDefault, cs.boxH + mousePosLast.y - mpos.y));
				sf::Vector2i p = { (int)pos.x, (int)pos.y };
				sf::Mouse::setPosition(p, app::mainWindow);
				mousePosLast = sf::Mouse::getPosition(app::mainWindow);
				sprDragL.setColor(color(c::light1));
				sprDragT.setColor(color(c::light1));

				if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
					dragLeft = false;
					dragTop = false;
					resizeStarted = false;
					init(false, true);
					app::mainWindow.setMouseCursorVisible(true);
				}
				else {
					init(false, false);
					app::mainWindow.setMouseCursorVisible(false);
				}
			}
			else if (dragLeft) {
				cs.boxW = std::max(cs.boxDefault, std::min((int)app::res.width - cs.boxDefault, cs.boxW + mousePosLast.x - mpos.x));
				sf::Vector2i p = { (int)pos.x, mousePosStart.y };
				sf::Mouse::setPosition(p, app::mainWindow);
				mousePosLast = sf::Mouse::getPosition(app::mainWindow);
				sprDragL.setColor(color(c::light1));

				if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
					dragLeft = false;
					resizeStarted = false;
					init(false, true);
					app::mainWindow.setMouseCursorVisible(true);
				}
				else {
					init(false, false);
					app::mainWindow.setMouseCursorVisible(false);
				}
			}
			else if (dragTop) {
				cs.boxH = std::max(cs.boxDefault, std::min((int)app::res.height - cs.boxDefault, cs.boxH + mousePosLast.y - mpos.y));
				sf::Vector2i p = { mousePosStart.x, (int)pos.y };
				sf::Mouse::setPosition(p, app::mainWindow);
				mousePosLast = sf::Mouse::getPosition(app::mainWindow);
				sprDragT.setColor(color(c::light1));

				if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
					dragTop = false;
					resizeStarted = false;
					init(false, true);
					app::mainWindow.setMouseCursorVisible(true);
				}
				else {
					init(false, false);
					app::mainWindow.setMouseCursorVisible(false);
				}
			}
		}
	}

	//---------------------------------------------------------
	// ScrollPanel
	void ScrollPanel::display() {
		texture.display();
		sprite.setTexture(texture.getTexture());
	}
	void ScrollPanel::create() {
		std::cout << "Recreating a ScrollPanel.\n";
		texture.create((unsigned int)size.x-1U, (unsigned int)size.y-1U);
		sprite.setTextureRect(sf::IntRect(0, 0, (int)size.x-1, (int)size.y-1));
	}
	void ScrollPanel::smooth() {
		scrollApp.x = approach(
			scrollApp.x, scroll.x, std::abs(scrollApp.x - scroll.x) / 3.f
		);
		scrollApp.y = approach(
			scrollApp.y, scroll.y, std::abs(scrollApp.y - scroll.y) / 3.f
		);
	}
	void ScrollPanel::check(int mx, int my) {
		if (input::scrollDelta != 0.f)
			if (hover(mx, my, pos.x, pos.y, size.x, size.y)) {
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt))
					return;
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
					input::scrollDelta *= 4;

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
					scroll.x += input::scrollDelta;
				else
					scroll.y += input::scrollDelta;
			}
	}
	void ScrollPanel::clear(sf::Color color) {
		texture.clear(color);
	}
	sf::RenderTexture& ScrollPanel::getTexture() {
		return texture;
	}


	void ScrollPanel::update() {
		sprite.setPosition(pos);
	}
	void ScrollPanel::draw(sf::RenderTarget& target) {
		if(!resizeStarted)
			target.draw(sprite);
	}

	//---------------------------------------------------------
	// Channels
	void Channels::check(int mx, int my) {

	}

	void Channels::update() {
		rect.setSize(cs.pat, cs.pat);
		rectOut.setSize(cs.pat, cs.pat);
		rect.setColor(color(c::outside, 175));
		rectOut.setColor(color(c::outline, 200));
		rect.update();
		rectOut.update();
		text.setFont(app::font);
		text.setCharacterSize(14);
	}
	void Channels::draw(sf::RenderTarget& target) {
		unsigned int w = app::project.songLength;
		unsigned int h = app::project.channels.size();

		// todo: CACHE SF::TEXT AND RECTANGLES IDIOT

		for (unsigned int j = 0U; j < h; j++) {
			for (unsigned int i = 0U; i < w; i++) {
				rectOut.setPos(
					std::round((i + panel->scrollApp.x) * cs.patFull + cs.patSpace),
					std::round((j + panel->scrollApp.y) * cs.patFull + cs.patSpace)
				);
				rectOut.update();
				rectOut.draw(target);
			}
		}
		for (unsigned int j = 0U; j < h; j++) {
			for (unsigned int i = 0U; i < w; i++) {
				rect.setPos(
					std::round((i + panel->scrollApp.x) * cs.patFull + cs.patSpace),
					std::round((j + panel->scrollApp.y) * cs.patFull + cs.patSpace)
				);
				rect.update();
				rect.draw(target);
			}
		}
		for (unsigned int j = 0U; j < h; j++) {
			for (unsigned int i = 0U; i < w; i++) {
				unsigned int v = app::project.channels[j]->slots[i] + (i + j) * 255 % 300;
				text.setFillColor(hsv(360.f - 360.f * ((j + 5) % 10 / 10.f), (v > 0 ? 80 : 50), (v > 0 ? 90 : 50)));
				text.setString(std::to_string(v));
				text.setPosition(
					std::round((i + panel->scrollApp.x) * cs.patFull + cs.patSpace + cs.pat / 2) - 1,
					std::round((j + panel->scrollApp.y) * cs.patFull + cs.patSpace + cs.pat / 2 - 14)
				);
				target.draw(text);
			}
		}
	}

	//---------------------------------------------------------
	// Render
	void Render::clear(sf::Color col) {
		texture.clear(col);
	}
	void Render::create(unsigned int w, unsigned int h) {
		texture.create(w, h);
		sprite.setTextureRect(sf::IntRect(0, 0, w, h));
	}
	void Render::display() {
		texture.display();
		sprite.setTexture(texture.getTexture());
	}
	void Render::draw(sf::RenderTarget& target) {
		target.draw(sprite);
	}
}
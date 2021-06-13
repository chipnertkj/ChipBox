#include "GUIClasses.h"	// interface
#include "GUI.h"
#include "GUIData.h"
#include "GUIUtility.h"
#include "App.h"
#include "Input.h"

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
	inline void Rectangle::draw(sf::RenderTarget& target) {
		target.draw(vert, vertCount, sf::Quads);
	}
	Rectangle::Rectangle(float x, float y, float w, float h) {
		pos = { x, y };
		size = { w, h };
		update();
	}
	Rectangle::Rectangle(sf::Vector2f p, sf::Vector2f s) {
		pos = p;
		size = s;
		update();
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
	inline void RectangleOut::draw(sf::RenderTarget& target) {
		target.draw(vert, vertCount, sf::LineStrip);
	}
	RectangleOut::RectangleOut(float x, float y, float w, float h) {
		pos = { x, y };
		size = { w, h };
		update();
	}
	RectangleOut::RectangleOut(sf::Vector2f p, sf::Vector2f s) {
		pos = p;
		size = s;
		update();
	}

	//---------------------------------------------------------
	// ProgressBar
	void ProgressBar::setTextures(sf::Texture& tx1) {
		shadow.setTexture(tx1);
	}
	void ProgressBar::updateValue() {
		if (refValue != NULL)
			if (vertical) {
				val.setSize(size.x, size.y * (*refValue / maxValue));
				val.setPos(pos.x, pos.y + (size.y - val.size.y));
			}
			else {
				val.setSize(size.x * (*refValue / maxValue), size.y);
				val.setPos(pos);
			}
		val.update();
	}
	void ProgressBar::update() {
		bg.set(pos, size);
		bg.setColor(th.progressBarBackground);
		bg.update();
		out.set(pos, size);
		out.setColor(th.outline);
		out.update();
		if (color)
			val.setColor(*color);
		shadow.setPosition(pos);
		sf::Vector2u tsize = shadow.getTexture()->getSize();
		shadow.setScale(size.x/tsize.x, size.y/tsize.y);
		shadow.setColor(sf::Color(255, 255, 255, 255));
	}
	void ProgressBar::draw(sf::RenderTarget& target) {
		bg.draw(target);
		val.draw(target);
		target.draw(shadow);
		out.draw(target);
	}

	//---------------------------------------------------------
	// Checkbox
	void Checkbox::setTextures(sf::Texture& tx1) {
		shadow.setTexture(tx1);
	}
	void Checkbox::updateValue() {
		if (refValue != NULL)
			val.setColor(*refValue ? th.channelMuted : *color);
	}
	void Checkbox::check(int mx, int my) {
		if (refValue != NULL)
			if (hover(mx, my, pos.x, pos.y, size.x, size.y))
				if (input::clickedLMB) {
					*refValue = !refValue;
					updateValue();
				}
	}
	void Checkbox::update() {
		bg.set(pos, size);
		bg.setColor(th.progressBarBackground);
		bg.update();
		out.set(pos, size);
		out.setColor(th.outline);
		out.update();
		val.set(pos, size);
		val.update();
		shadow.setPosition(pos);
		sf::Vector2u tsize = shadow.getTexture()->getSize();
		shadow.setScale(size.x / tsize.x, size.y / tsize.y);
		shadow.setColor(sf::Color(255, 255, 255, 255));
	}
	void Checkbox::draw(sf::RenderTarget& target) {
		bg.draw(target);
		val.draw(target);
		target.draw(shadow);
		out.draw(target);
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
	bool Button::check(int mx, int my) {
		checked = false;
		mx += (int)size.x / 2;
		my += (int)size.y / 2;
		if (hover(mx, my, pos.x, pos.y, size.x, size.y)) {
			checked = true;
			app::mainWindow.setMouseCursor(app::cursor_hand);
			app::cursorChanged = true;
		}
		return checked;
	}
	void Button::update() {
		getSprite().setColor(th.buttonInactive);
		getHighlight().setColor(th.buttonActive);
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
		getSprite().setColor(checked ? th.buttonActive : th.buttonInactive);
		sf::Vector2f sc = getHighlight().getScale();
		float v = (checked ? (defScale.x * 0.5f) : 0.f);
		getHighlight().setScale(
			approach(sc.x, v, std::abs(sc.x - v) / 2.f),
			sc.y
		);
		sc = getHighlight().getScale();
		getHighlight().setColor(withAlpha(th.buttonActive, sf::Uint8(std::min(1.f, (float)pow(sc.x/(defScale.x*0.5f), 0.4f))*255U)));
	}

	//---------------------------------------------------------
	// Widget
	void Widget::update() {
		// bg
		bg.set(pos, size);
		bg.setColor(th.boxOutside);
		bg.update();
		// bg outline
		bgOut.set(pos, size);
		bgOut.setColor(th.outline);
		bgOut.update();
		// front
		front.set(
			pos.x + cs.space, pos.y + cs.space,
			size.x - cs.space * 2, size.y - bottom * cs.space - cs.space
		);
		front.setColor(th.boxInside);
		front.update();
		// front outline
		frontOut.set(front.pos, front.size);
		frontOut.setColor(th.outline);
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
	void Widget::handlesSetup() {
		sprDragL.setTexture(app::tx_dragHandle);
		sprDragL.setOrigin(sf::Vector2f(app::tx_dragHandle.getSize() / 2U));
		sprDragT.setTexture(app::tx_dragHandle);
		sprDragT.setOrigin(sf::Vector2f(app::tx_dragHandle.getSize() / 2U));
		sprDragL.setColor(th.outline);
		sprDragT.setColor(th.outline);
	}
	void Widget::createScrollPanel() {
		scrollPanel = new ScrollPanel;
		scroll = true;
	}
	void Widget::setHandlesScale(float sx, float sy) {
		sprDragL.setScale(sx, sy);
		sprDragT.setScale(sx, sy);
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
		if (!cursorDragStarted) {
			if (checkLeft)
				if (hover(mx, my, pos.x, pos.y, cs.space, size.y)) {
					mousePosStart = sf::Mouse::getPosition(app::mainWindow);
					passLeft = true;
					if (input::clickedMMB) {
						if (cs.boxW != cs.boxBoundsScaled)
							cs.boxW = cs.boxBoundsScaled;
						else
							cs.boxW = app::res.width - cs.boxBoundsScaled;
						init(false, true);
					}
					else if (input::clickedLMB) {
						dragLeft = true;
						mousePosLast = sf::Mouse::getPosition(app::mainWindow);
					}
				}
			if (checkTop)
				if (hover(mx, my, pos.x, pos.y, size.x, cs.space)) {
					mousePosStart = sf::Mouse::getPosition(app::mainWindow);
					passTop = true;
					if (input::clickedMMB) {
						if (cs.boxH != cs.boxBoundsScaled)
							cs.boxH = cs.boxBoundsScaled;
						else
							cs.boxH = app::res.width - cs.boxBoundsScaled;
						init(false, true);
					}
					else if (input::clickedLMB) {
						dragTop = true;
						mousePosLast = sf::Mouse::getPosition(app::mainWindow);
					}
				}
		}

		if (dragLeft || dragTop) {
			input::clickedLMB = false;
			cursorDragStarted = true;
			guiScalingStarted = true;
		}
		else
			if (!cursorDragStarted) {
				sprDragL.setColor(th.outline);
				sprDragT.setColor(th.outline);
			}

		if (!app::cursorChanged) {
			if (passLeft && passTop) {
				app::mainWindow.setMouseCursor(app::cursor_tleftbright);
				sprDragL.setColor(th.buttonActive);
				sprDragT.setColor(th.buttonActive);
				app::cursorChanged = true;
			}
			else if (passLeft) {
				app::mainWindow.setMouseCursor(app::cursor_sizeH);
				sprDragL.setColor(th.buttonActive);
				app::cursorChanged = true;
			}
			else if (passTop) {
				app::mainWindow.setMouseCursor(app::cursor_sizeV);
				sprDragT.setColor(th.buttonActive);
				app::cursorChanged = true;
			}
		}

		sf::Vector2i mpos = sf::Mouse::getPosition(app::mainWindow);

		if (mousePosLast != mpos || !sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			if (dragLeft && dragTop) {
				cs.boxW = std::max(cs.boxBoundsScaled, std::min((int)app::res.width - cs.boxBoundsScaled, cs.boxW + mousePosLast.x - mpos.x));
				cs.boxH = std::max(cs.boxBoundsScaled, std::min((int)app::res.height - cs.boxBoundsScaled, cs.boxH + mousePosLast.y - mpos.y));
				sf::Mouse::setPosition(sf::Vector2i{ (int)pos.x, (int)pos.y }, app::mainWindow);
				mousePosLast = sf::Mouse::getPosition(app::mainWindow);
				sprDragL.setColor(th.buttonActive);
				sprDragT.setColor(th.buttonActive);

				if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
					dragLeft = false;
					dragTop = false;
					cursorDragStarted = false;
					guiScalingStarted = false;
					init(false, true);
					app::mainWindow.setMouseCursorVisible(true);
				}
				else {
					init(false, false);
					app::mainWindow.setMouseCursorVisible(false);
				}
			}
			else if (dragLeft) {
				cs.boxW = std::max(cs.boxBoundsScaled, std::min((int)app::res.width - cs.boxBoundsScaled, cs.boxW + mousePosLast.x - mpos.x));
				sf::Mouse::setPosition(sf::Vector2i{ (int)pos.x, mousePosStart.y }, app::mainWindow);
				mousePosLast = sf::Mouse::getPosition(app::mainWindow);
				sprDragL.setColor(th.buttonActive);

				if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
					dragLeft = false;
					cursorDragStarted = false;
					guiScalingStarted = false;
					init(false, true);
					app::mainWindow.setMouseCursorVisible(true);
				}
				else {
					init(false, false);
					app::mainWindow.setMouseCursorVisible(false);
				}
			}
			else if (dragTop) {
				cs.boxH = std::max(cs.boxBoundsScaled, std::min((int)app::res.height - cs.boxBoundsScaled, cs.boxH + mousePosLast.y - mpos.y));
				sf::Mouse::setPosition(sf::Vector2i{ mousePosStart.x, (int)pos.y }, app::mainWindow);
				mousePosLast = sf::Mouse::getPosition(app::mainWindow);
				sprDragT.setColor(th.buttonActive);

				if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
					dragTop = false;
					cursorDragStarted = false;
					guiScalingStarted = false;
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
		app::cslog("GUI", "Recreating a ScrollPanel.");
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
		// scrolling
		if (input::scrollDelta != 0.f) {
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

		// dragging
		bool pass = false;
		if (!cursorDragStarted) {
			if (hover(mx, my, pos.x, pos.y, size.x, size.y)) {
				mousePosStart = sf::Mouse::getPosition(app::mainWindow);
				pass = true;
				if (input::clickedRMB) {
					drag = true;
					mousePosLast = sf::Mouse::getPosition(app::mainWindow);
				}
			}
		}
		sf::Vector2i mpos = sf::Mouse::getPosition(app::mainWindow);
		if (mousePosLast != mpos || cursorDragStarted) {
			if (drag) {
				scroll.x += (mousePosLast.x - mpos.x)/((float)*scrollScale);
				scroll.y += (mousePosLast.y - mpos.y)/((float)*scrollScale);
				sf::Mouse::setPosition(sf::Vector2i{ (int)mousePosStart.x, (int)mousePosStart.y }, app::mainWindow);
				mousePosLast = sf::Mouse::getPosition(app::mainWindow);

				if (!sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
					drag = false;
					cursorDragStarted = false;
					app::mainWindow.setMouseCursorVisible(true);
				}
				else {
					cursorDragStarted = true;
					app::mainWindow.setMouseCursorVisible(false);
				}
			}
		}
	}
	void ScrollPanel::clamp() {
		scroll.x = std::max(std::min(scroll.x, scrollMax.x), 0.f);
		scroll.y = std::max(std::min(scroll.y, scrollMax.y), 0.f);
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
	inline void ScrollPanel::draw(sf::RenderTarget& target) {
		if (!guiScalingStarted)
			target.draw(sprite);
	}

	//---------------------------------------------------------
	// Channels
	// TODO: (BUG) fix incorrectly displayed slots at specific scroll values
	void Channels::recalculate() {
		// calculate
		unsigned int prevw = width;
		unsigned int prevh = height;
		width = std::min(proj::project.getSongLength(), unsigned int(std::ceil(scrollPanel->size.x / cs.patFull)));
		height = std::min(unsigned int(proj::project.channels.size()), unsigned int(std::ceil(scrollPanel->size.y / cs.patFull))) + 1U;

		scrollPanel->scrollMax.x = std::max(0.f, (float)proj::project.getSongLength() - (scrollPanel->size.x - cs.patSpace - 1) / cs.patFull);
		scrollPanel->scrollMax.y = std::max(0.f, (float)proj::project.channels.size() - (scrollPanel->size.y - cs.patSpace - 1) / cs.patFull);

		// visual fix (add at the end if outside widget)
		if (scrollPanel->scrollMax.x > 0.f)
			width++;
		if (scrollPanel->scrollMax.y > 0.f)
			height++;

		// skip if no changes
		if (prevw == width && prevh == height)
			return;

		app::cslog("GUI", std::string("Allocating channel slots: ") + std::to_string(width) + "x" + std::to_string(height));
		// deallocate
		delete[] rect;
		delete[] rectOut;
		delete[] text;
		delete[] volume;
		delete[] muted;
		// allocate
		rect = new Rectangle[width * height];
		rectOut = new RectangleOut[width * height];
		text = new sf::Text[width * height];
		volume = new ProgressBar[height];
		muted = new Checkbox[height];

		// update
		for (unsigned int y = 0U; y < height; y++) {
			muted[y].setSize(cs.pat / 2.f - cs.patSpace, cs.pat / 2.f - cs.patSpace);
			muted[y].setTextures(app::tx_shadow_box);
			volume[y].setSize(cs.pat / 2.f - cs.patSpace, cs.pat);
			volume[y].setTextures(app::tx_shadow_v);
		}
		for (unsigned int y = 0U; y < height; y++)
			for (unsigned int x = 0U; x < width; x++) {
				rect[width * y + x].setSize(cs.pat, cs.pat);
				rectOut[width * y + x].setSize(cs.pat, cs.pat);
			}
		for (unsigned int y = 0U; y < height; y++)
			for (unsigned int x = 0U; x < width; x++) {
				rect[width * y + x].setColor(th.boxOutside);
				rectOut[width * y + x].setColor(th.outline);
			}
		for (unsigned int y = 0U; y < height; y++)
			for (unsigned int x = 0U; x < width; x++) {
				text[width * y + x].setFont(app::font);
				text[width * y + x].setCharacterSize(15u*cs.scale);
			}
		update();
	}
	void Channels::refresh() {
		unsigned int offx = std::ceil(scrollPanel->scrollApp.x - 0.5f);
		unsigned int offy = std::ceil(scrollPanel->scrollApp.y - 0.5f);
		for (unsigned int i = 0U; i < width * height; i++) {
			unsigned int px = std::ceil(rect[i].pos.x / cs.patFull - 0.5f) + offx - 1;
			unsigned int py = std::ceil(rect[i].pos.y / cs.patFull - 0.5f) + offy;
			if (px < proj::project.getSongLength())
				if (py < proj::project.channels.size())
					text[i].setString(std::to_string(proj::project.channels[py]->slots[px]));
		}
	}
	void Channels::update() {
		// update
		scale = cs.patFull;
		// code
		double sx = scrollPanel->scrollApp.x;
		double sy = scrollPanel->scrollApp.y;
		sx = sx - int(sx);
		sy = sy - int(sy);
		sx = std::floor(sx * cs.patFull);
		sy = std::floor(sy * cs.patFull);

		for (unsigned int y = 0U; y < height; y++)
			for (unsigned int x = 0U; x < width; x++) {
				rect[width * y + x].setPos((x + 1) * cs.patFull - sx + cs.patSpace, (y - 1) * cs.patFull - sy + cs.patSpace);
				rect[width * y + x].update();
			}
		for (unsigned int y = 0U; y < height; y++)
			for (unsigned int x = 0U; x < width; x++) {
				rectOut[width * y + x].setPos((x + 1) * cs.patFull - sx + cs.patSpace, (y - 1) * cs.patFull - sy + cs.patSpace);
				rectOut[width * y + x].update();
			}
		for (unsigned int y = 0U; y < height; y++)
			for (unsigned int x = 0U; x < width; x++)
				text[width * y + x].setPosition((x + 1) * cs.patFull - sx + cs.patSpace, (y - 1) * cs.patFull - sy + cs.patSpace);
		for (unsigned int y = 0U; y < height; y++) {
			volume[y].setPos(cs.patSpace, (y - 1) * cs.patFull - sy + cs.patSpace);
			volume[y].update();
		}
		for (unsigned int y = 0U; y < height; y++) {
			muted[y].setPos(cs.patSpace + cs.pat/2.f, (y - 1) * cs.patFull - sy + cs.patSpace);
			muted[y].update();
		}
	}

	void Channels::draw(sf::RenderTarget& target) {
		for (unsigned int y = 0U; y < height; y++)
			for (unsigned int x = 0U; x < width; x++) {
				rect[width * y + x].draw(target);  // TODO: (OPTIMIZATION, 10%) cache to a texture on recalculate() and draw that instead
			}
		for (unsigned int y = 0U; y < height; y++)
			for (unsigned int x = 0U; x < width; x++) {
				rectOut[width * y + x].draw(target);  // TODO: (OPTIMIZATION, 12%) cache to a texture on recalculate() and draw that instead
			}
		for (unsigned int y = 0U; y < height; y++)
			for (unsigned int x = 0U; x < width; x++) {
				target.draw(text[width * y + x]); // TODO: (OPTIMIZATION, 46%) cache to a texture on refresh() and draw that instead
			}
		for (unsigned int y = 0U; y < height; y++)
			volume[y].draw(target);
		for (unsigned int y = 0U; y < height; y++)
			muted[y].draw(target);
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
	inline void Render::draw(sf::RenderTarget& target) {
		target.draw(sprite);
	}
}
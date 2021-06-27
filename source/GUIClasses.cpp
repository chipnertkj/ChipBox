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
		vert[0].position.x = pos.x - 1;			vert[0].position.y = pos.y;
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
		shadow.setScale(size.x / tsize.x, size.y / tsize.y);
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
				if (input::onClick(input::clickedLMB)) {
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
	void Button::setTextures(sf::Texture& tx1, sf::Texture& tx2) {
		getSprite().setTexture(tx1);
		getSprite().setOrigin((sf::Vector2f)tx1.getSize() / 2.f);
		getHighlight().setTexture(tx2);
		getHighlight().setOrigin((sf::Vector2f)tx2.getSize() / 2.f);
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
		getSprite().setScale(sx * 0.7f, sy * 0.7f);
		getHighlight().setScale(0.f, sy * 0.5f);
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
		getHighlight().setColor(withAlpha(th.buttonActive, sf::Uint8(std::min(1.f, (float)pow(sc.x / (defScale.x * 0.5f), 0.4f)) * 255U)));
	}

	//---------------------------------------------------------
	// Graph
	void Graph::createLine(unsigned int _pointCount) {
		delete[] line;
		delete[] points;
		line = new sf::Vertex[_pointCount];
		points = new sf::Vector2f[_pointCount];
		pointCount = _pointCount;
	}
	void Graph::setPoint(unsigned int _point, float _x, float _y) {
		_x = -std::max(std::min(_x, 1.f), -1.f);
		_y = -std::max(std::min(_y, 1.f), -1.f);
		points[_point] = { _x, _y };
	}
	void Graph::setTextures(sf::Texture& tx1) {
		shadow.setTexture(tx1);
	}
	void Graph::updateLine() {
		for (size_t i = 0; i < pointCount; i++) {
			line[i].position.x = front.pos.x + ((float)front.size.x * (points[i].x + 1));
			line[i].position.y = front.pos.y + ((float)front.size.y / 2 * (1.f - points[i].y));
		}
	}
	void Graph::update() {
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
			pos.x + cs.modSpace, pos.y + cs.modSpace,
			size.x - cs.modSpace * 2, size.y - cs.modSpace * 2
		);
		front.setColor(th.moduleBg);
		front.update();
		// front outline
		frontOut.set(front.pos, front.size);
		frontOut.setColor(th.outline);
		frontOut.update();
		// shadow
		shadow.setPosition(front.pos);
		sf::Vector2u tsize = shadow.getTexture()->getSize();
		shadow.setScale(front.size.x / tsize.x, front.size.y / tsize.y);
		shadow.setColor(sf::Color(255, 255, 255, 128));
	}
	void Graph::draw(sf::RenderTarget& target) {
		bg.draw(target);
		front.draw(target);
		target.draw(line, pointCount, sf::LineStrip);
		target.draw(shadow);
		bgOut.draw(target);
		frontOut.draw(target);
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
	void Widget::check(int mx, int my) {
		if (!(checkTop || checkLeft))
			return;
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
							cs.boxH = app::res.height - cs.boxBoundsScaled - cs.topH;
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
				cs.boxH = std::max(cs.boxBoundsScaled, std::min((int)app::res.height - cs.boxBoundsScaled - cs.topH, cs.boxH + mousePosLast.y - mpos.y));
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
				cs.boxH = std::max(cs.boxBoundsScaled, std::min((int)app::res.height - cs.boxBoundsScaled - cs.topH, cs.boxH + mousePosLast.y - mpos.y));
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
		cslogstr("GUI", "Recreating a ScrollPanel.")
		texture.create((unsigned int)size.x - 1U, (unsigned int)size.y - 1U);
		sprite.setTextureRect(sf::IntRect(0, 0, (int)size.x - 1, (int)size.y - 1));
	}
	bool ScrollPanel::smooth() {
		float x = scrollApp.x, y = scrollApp.y;
		scrollApp.x = approach(
			scrollApp.x, scroll.x, std::abs(scrollApp.x - scroll.x) / 3.f
		);
		scrollApp.y = approach(
			scrollApp.y, scroll.y, std::abs(scrollApp.y - scroll.y) / 3.f
		);
		if (x != scrollApp.x)
			return true;
		if (y != scrollApp.y)
			return true;
		return false;
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
				scroll.x += (mousePosLast.x - mpos.x) / ((float)*scrollScale);
				scroll.y += (mousePosLast.y - mpos.y) / ((float)*scrollScale);
				mousePosLast = sf::Mouse::getPosition(app::mainWindow);

				if (!sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
					drag = false;
					cursorDragStarted = false;
				}
				else {
					cursorDragStarted = true;
				}
			}
		}
	}
	void ScrollPanel::clamp() {
		scroll.x = std::max(std::min(scroll.x, scrollMax.x), 0.f);
		scroll.y = std::max(std::min(scroll.y, scrollMax.y), 0.f);
		scrollApp.x = std::max(std::min(scrollApp.x, scrollMax.x), 0.f);
		scrollApp.y = std::max(std::min(scrollApp.y, scrollMax.y), 0.f);
	}
	void ScrollPanel::clear(sf::Color color) {
		texture.clear(color);
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
	void Channels::cacheBar() {
		cslogstr("GUI", "Caching channel bar")
		for (unsigned int y = 0U; y < height; y++) {
			volume[y].setPos(cs.patSpace, (y - 1) * cs.patFull + cs.patSpace);
			volume[y].round();
			volume[y].update();
		}
		for (unsigned int y = 0U; y < height; y++) {
			muted[y].setPos(cs.patSpace + cs.pat / 2.f, (y - 1) * cs.patFull + cs.patSpace);
			muted[y].round();
			muted[y].update();
		}
	}
	void Channels::cacheRects() {
		cslogstr("GUI", "Caching channel rects")
		renderRect.clear(sf::Color(0, 0, 0, 0));
		/// adjust
		for (unsigned int y = 0U; y < height; y++)
			for (unsigned int x = 0U; x < width; x++) {
				rect[width * y + x].setPos((x + 1) * cs.patFull + cs.patSpace, (y - 1) * cs.patFull + cs.patSpace);
				rect[width * y + x].round();
				rect[width * y + x].update();
			}
		for (unsigned int y = 0U; y < height; y++)
			for (unsigned int x = 0U; x < width; x++) {
				rectOut[width * y + x].setPos((x + 1) * cs.patFull + cs.patSpace, (y - 1) * cs.patFull + cs.patSpace);
				rectOut[width * y + x].round();
				rectOut[width * y + x].update();
			}
		/// draw
		// rects
		for (unsigned int y = 0U; y < height; y++)
			for (unsigned int x = 0U; x < width; x++) {
				rect[width * y + x].draw(renderRect.texture);
			}
		for (unsigned int y = 0U; y < height; y++)
			for (unsigned int x = 0U; x < width; x++) {
				rectOut[width * y + x].draw(renderRect.texture);
			}
		// display
		renderRect.display();
	}
	void Channels::cacheText() {
		cslogstr("GUI", "Caching channel text")
		renderText.clear(sf::Color(0, 0, 0, 0));
		// adjust
		sf::FloatRect bounds;
		for (unsigned int y = 0U; y < height; y++)
			for (unsigned int x = 0U; x < width; x++) {
				bounds = text[width * y + x].getLocalBounds();
				text[width * y + x].setPosition(
					x * cs.patFull + cs.patSpace + std::floor(cs.patFull / 2.f - (bounds.left + bounds.width) / 2.f) - 2,
					(y - 1) * cs.patFull + cs.patSpace + std::floor(cs.patFull / 2.f - (15u * cs.scale / 2.f) - 3)
				);
			}
		// draw
		for (unsigned int y = 0U; y < height; y++)
			for (unsigned int x = 0U; x < width; x++) {
				renderText.texture.draw(text[width * y + x], textBlendMode);
			}
		// display
		renderText.display();
	}
	void Channels::recalculate(bool rescale) {
		// calculate
		unsigned int prevw = width;
		unsigned int prevh = height;
		width = std::min(proj::project.getSongLength(), unsigned int(std::ceil(scrollPanel->size.x / cs.patFull))) + 1u;
		height = std::min(unsigned int(proj::project.getChannelCount()), unsigned int(std::ceil(scrollPanel->size.y / cs.patFull))) + 1u;

		scrollPanel->scrollMax.x = std::max(0.f, (float)(proj::project.getSongLength() + 1) - (scrollPanel->size.x - cs.patSpace - 1) / cs.patFull);
		scrollPanel->scrollMax.y = std::max(0.f, (float)proj::project.getChannelCount() - (scrollPanel->size.y - cs.patSpace - 1) / cs.patFull);

		// visual fix (add at the end if outside widget)
		if (scrollPanel->scrollMax.x > 0.f)
			width++;
		if (scrollPanel->scrollMax.y > 0.f)
			height++;

		// skip if no changes
		if (!rescale)
			if (prevw == width && prevh == height)
				return;

		// recreate rect render
		renderRect.create(width * cs.patFull, height * cs.patFull);
		renderText.create(width * cs.patFull, height * cs.patFull);
		renderBar.create(cs.patFull, height * cs.patFull);

		// deallocate
		delete[] rect;
		delete[] rectOut;
		delete[] text;
		delete[] volume;
		delete[] muted;
		// allocate
		cslogstr("GUI", std::string("Allocating channel slots: ") + std::to_string(width) + "x" + std::to_string(height))
		rect = new Rectangle[width * height];
		rectOut = new RectangleOut[width * height];
		text = new sf::Text[width * height];
		volume = new ProgressBar[height];
		muted = new Checkbox[height];

		// update
		for (unsigned int y = 0U; y < height; y++) {
			muted[y].setSize(cs.pat / 2.f - cs.patSpace, cs.pat / 2.f - cs.patSpace);
			muted[y].setTextures(app::tx_shadow_box);
		}
		for (unsigned int y = 0U; y < height; y++) {
			volume[y].setSize(cs.pat / 2.f - cs.patSpace, cs.pat);
			volume[y].setTextures(app::tx_shadow_v);
		}
		for (unsigned int y = 0U; y < height; y++)
			for (unsigned int x = 0U; x < width; x++) {
				rect[width * y + x].setSize(cs.pat, cs.pat);
				rect[width * y + x].setColor(th.boxOutside);
				rect[width * y + x].update();
			}
		for (unsigned int y = 0U; y < height; y++)
			for (unsigned int x = 0U; x < width; x++) {
				rectOut[width * y + x].setSize(cs.pat, cs.pat);
				rectOut[width * y + x].setColor(th.outline);
				rectOut[width * y + x].update();
			}
		for (unsigned int y = 0U; y < height; y++)
			for (unsigned int x = 0U; x < width; x++) {
				text[width * y + x].setFont(app::font);
				text[width * y + x].setCharacterSize(15u * cs.scale);
			}

		// cache
		cacheRects();
		// cacheText() not needed (refresh())
		cacheBar();
		refresh();
		updateChannels();
	}
	void Channels::refresh() {
		unsigned int offx = unsigned int(scrollPanel->scrollApp.x);
		unsigned int offy = unsigned int(scrollPanel->scrollApp.y);
		for (unsigned int i = 0U; i < width * height; i++) {
			unsigned int x = std::floor(rect[i].pos.x / cs.patFull) + offx - 2;
			unsigned int y = std::floor(rect[i].pos.y / cs.patFull) + offy;
			if (y < proj::project.getChannelCount())
				if (x < proj::project.getSongLength())
					text[i].setString(std::to_string(proj::project.getChannel(y)->slots[x]));
		}
		needRedraw = true;
		cacheText();
	}
	void Channels::update() {
		selectedRect[0].setSize(widget->getInsideSize().x, cs.pat + 2);
		selectedRect[0].setColor(th.selectedBG);
		selectedRect[1].setSize(cs.pat + 2, widget->getInsideSize().y);
		selectedRect[1].setColor(th.selectedBG);
		selectedRect[2].setSize(cs.pat+3, cs.pat+3);
		selectedRect[2].setColor(th.selected);
		selectedRect[3].setSize(cs.pat + 3, cs.pat + 3);
		selectedRect[3].setColor(th.selected);
		updateChannels();
	}
	void Channels::setTextures(sf::Texture& tx1) {
		for (int i = 0; i < 4; i++)
			shadow[i].setTexture(tx1);
		shadow[1].setRotation(180);
		shadow[2].setRotation(270);
		shadow[3].setRotation(90);
	}
	void Channels::updateChannels() {
		double sx = scrollPanel->scrollApp.x;
		double sy = scrollPanel->scrollApp.y;
		if (lastx != int(sx) || lasty != int(sy))
			refresh();
		lastx = int(sx);
		lasty = int(sy);
		sx = sx - lastx;
		sy = sy - lasty;
		sx = std::floor(sx * (double)cs.patFull);
		sy = std::floor(sy * (double)cs.patFull);

		renderRect.pos.x = -sx; renderRect.pos.y = -sy;
		renderText.pos.x = -sx; renderText.pos.y = -sy;
		renderBar.pos.y = -sy;
		renderRect.update();
		renderText.update();
		renderBar.update();

		selectedRect[0].setPos(cs.patFull, (selected.y - scrollPanel->scrollApp.y) * cs.patFull + cs.patSpace - 2);
		selectedRect[0].round();
		selectedRect[0].update();
		selectedRect[1].setPos((selected.x + 1 - scrollPanel->scrollApp.x) * cs.patFull + cs.patSpace - 2, 0);
		selectedRect[1].round();
		selectedRect[1].update();
		selectedRect[2].setPos(
			(selected.x - scrollPanel->scrollApp.x + 1) * cs.patFull + cs.patSpace - 2,
			(selected.y - scrollPanel->scrollApp.y) * cs.patFull + cs.patSpace - 2
		);
		selectedRect[2].round();
		selectedRect[2].update();

		sf::Vector2f ws = widget->getInsideSize();
		shadow[0].setScale(ws.x - cs.patFull, cs.scale);
		shadow[0].setPosition(cs.patFull, 0);
		shadow[1].setScale(ws.x - cs.patFull, cs.scale);
		shadow[1].setPosition(ws.x, ws.y);
		shadow[2].setScale(ws.y, cs.scale);
		shadow[2].setPosition(cs.patFull, ws.y);
		shadow[3].setScale(ws.y, cs.scale);
		shadow[3].setPosition(ws.x, 0);
		
		barLine[0].position = { (float)cs.patFull, 0 };
		barLine[0].color = th.outline;
		barLine[1].position = { (float)cs.patFull, ws.y };
		barLine[1].color = th.outline;

		if (scrollPanel->smooth())
			updateCursor(cx, cy); // includes needRedraw = true
	}
	void Channels::draw(sf::RenderTarget& target) {
		/// check
		if (!needRedraw)
			return;
		needRedraw = false;
		/// draw
		scrollPanel->clear(th.boxInside);
		// channels
		renderRect.draw(target);
		renderText.draw(target);
		// selected
		for (int i = 0; i < 3; i++)
			selectedRect[i].draw(target);
		if (showCursor)
			selectedRect[3].draw(target);
		// shadows
		for (int i = 0; i < 4; i++)
			target.draw(shadow[i]);
		// volume, muting
		renderBar.clear(th.boxInside);
		for (unsigned int y = 0U; y < height; y++)
			volume[y].draw(renderBar.texture);
		for (unsigned int y = 0U; y < height; y++)
			muted[y].draw(renderBar.texture);
		renderBar.display();
		renderBar.draw(target);
		target.draw(barLine, 2, sf::Lines);
	}
	void Channels::check(int mx, int my) {
		// scroll panel
		scrollPanel->check(mx, my);
		scrollPanel->clamp();

		// mouse
		int x = 0, y = 0;
		sf::Vector2f wp = widget->getInsidePos(), ws = widget->getInsideSize();
		// the = is here on purpose, don't cry
		if (showCursor = hover(mx, my, wp.x + cs.patFull, wp.y, ws.x - cs.patFull, ws.y)) {
			x = std::floor((mx - wp.x) / cs.patFull + scrollPanel->scrollApp.x) - 1;
			y = std::floor((my - wp.y) / cs.patFull + scrollPanel->scrollApp.y);

			// mouse click
			if (input::onClick(input::clickedLMB))
				if (x < (int)proj::project.getSongLength())
					if (y < (int)proj::project.getChannelCount())
						setSelected(x, y);

			// update cache
			if (x != lastccx || y != lastccy)
				updateCursor(x, y);
		}
		else {
			if (showCursor != showCursorLast) {
				needRedraw = true;
			}
		}

		lastccx = x;
		lastccy = y;
		showCursorLast = showCursor;
	}
	void Channels::updateCursor(int _x, int _y) {
		cx = _x; cy = _y;
		if ((cx > (int)proj::project.getSongLength() - 1) || (cy > (int)proj::project.getChannelCount() - 1))
			showCursor = false;
		selectedRect[3].setPos(
			(_x + 1 - scrollPanel->scrollApp.x) * cs.patFull + cs.patSpace - 2,
			(_y - scrollPanel->scrollApp.y) * cs.patFull + cs.patSpace - 2
		);
		selectedRect[3].round();
		selectedRect[3].update();
		needRedraw = true;
	}
	void Channels::setSelected(int _x, int _y) {
		selected = { _x, _y };
		selected.x = std::clamp(selected.x, 0, (int)proj::project.getSongLength() - 1);
		selected.y = std::clamp(selected.y, 0, (int)proj::project.getChannelCount() - 1);
		pan();
	}
	void Channels::moveSelected(int _x, int _y) {
		selected += { _x, _y };
		// x
		if (selected.x > (int)proj::project.getSongLength() - 1)
			selected.x = 0;
		if (selected.x < 0)
			selected.x = (int)proj::project.getSongLength() - 1;
		// y
		if (selected.y > (int)proj::project.getChannelCount() - 1)
			selected.y = 0;
		if (selected.y < 0)
			selected.y = (int)proj::project.getChannelCount() - 1;
		pan();
	}
	void Channels::pan() {
		scrollPanel->scroll.x = selected.x - ((std::round(widget->getInsideSize().x / cs.patFull) - 1) / 2);
		scrollPanel->scroll.y = selected.y - ((std::round(widget->getInsideSize().y / cs.patFull) - 1) / 2);
		scrollPanel->clamp();
		needRedraw = true;
	}
	void Channels::resetView() {
		pan();
		scrollPanel->scrollApp.x = selected.x - ((std::round(widget->getInsideSize().x / cs.patFull) - 1) / 2);
		scrollPanel->scrollApp.y = selected.y - ((std::round(widget->getInsideSize().y / cs.patFull) - 1) / 2);
		scrollPanel->clamp();
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
	void Render::update() {
		sprite.setPosition(pos);
	}
	void Render::draw(sf::RenderTarget& target) {
		target.draw(sprite);
	}
}
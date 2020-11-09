#pragma once
#include "InterfaceFunctions.h"

class Drawable {
public:
	float x, y, w, h;

	Drawable() {};

	void initPos() {};
	void initTheme() {};
	void draw() {};
};

class Menu : public Drawable {
private:
	float spacing = 1.1f;

	sf::Vertex fill[4];
	sf::Vertex outline[2];

public:
	Menu() {};

	void initPos() {
		x = 0.0f; y = 0.0f; w = (float)res.x; h = (float)res.y / 40.0f;

		// back
		fill[0].position.x = x;		fill[0].position.y = y;
		fill[1].position.x = x + w; fill[1].position.y = y;
		fill[2].position.x = x + w; fill[2].position.y = y + h;
		fill[3].position.x = x;		fill[3].position.y = y + h;

		// line
		outline[0].position.x = x; outline[0].position.y = y + h;
		outline[1].position.x = x + w; outline[1].position.y = y + h;

		// floor
		for (int i = 0; i < 4; i++) {
			fill[i].position.x = std::floor(fill[i].position.x);
			fill[i].position.y = std::floor(fill[i].position.y);
		}
		for (int i = 0; i < 2; i++) {
			outline[i].position.x = std::floor(outline[i].position.x);
			outline[i].position.y = std::floor(outline[i].position.y);
		}
	}

	void initTheme() {
		sf::Color col = color(c::inside, 255);
		for (int i = 0; i < 4; i++)
			fill[i].color = col;

		col = color(c::outline, 255);
		for (int i = 0; i < 2; i++)
			outline[i].color = col;
	};

	void draw(sf::RenderTarget& target) {
		target.draw(fill, 4, sf::Quads);
		target.draw(outline, 2, sf::Lines);
	}
};

// class for drawable box containers
class Container : public Drawable {
private:
	float spacing = 1.1f;

	sf::Vertex back[4];
	sf::Vertex front[4];
	sf::Vertex backline[8];
	sf::Vertex frontline[8];
public:
	float bottom = 5.0f;

	Container() {}

	void set(float _x, float _y, float _w, float _h) {
		x = _x; y = _y; w = _w; h = _h;
	}

	void initPos() {
		// back
		back[0].position.x = x;		back[0].position.y = y;
		back[1].position.x = x + w; back[1].position.y = y;
		back[2].position.x = x + w; back[2].position.y = y + h;
		back[3].position.x = x;		back[3].position.y = y + h;
		// front
		front[0].position.x = x + out * spacing;		front[0].position.y = y + out * spacing;
		front[1].position.x = x + w - out * spacing;	front[1].position.y = y + out * spacing;
		front[2].position.x = x + w - out * spacing;	front[2].position.y = y + h - out * (spacing + bottom);
		front[3].position.x = x + out * spacing;		front[3].position.y = y + h - out * (spacing + bottom);
		// backline
		backline[0].position.x = x - 1;		backline[0].position.y = y;
		backline[1].position.x = x + w; backline[1].position.y = y;
		backline[2].position.x = x + w; backline[2].position.y = y;
		backline[3].position.x = x + w; backline[3].position.y = y + h;
		backline[4].position.x = x + w; backline[4].position.y = y + h;
		backline[5].position.x = x;		backline[5].position.y = y + h;
		backline[6].position.x = x;		backline[6].position.y = y + h;
		backline[7].position.x = x;		backline[7].position.y = y;
		// frontline
		frontline[0].position.x = x + out * spacing - 1;	frontline[0].position.y = y + out * spacing;
		frontline[1].position.x = x + w - out * spacing;	frontline[1].position.y = y + out * spacing;
		frontline[2].position.x = x + w - out * spacing;	frontline[2].position.y = y + out * spacing;
		frontline[3].position.x = x + w - out * spacing;	frontline[3].position.y = y + h - out * (spacing + bottom);
		frontline[4].position.x = x + w - out * spacing;	frontline[4].position.y = y + h - out * (spacing + bottom);
		frontline[5].position.x = x + out * spacing;		frontline[5].position.y = y + h - out * (spacing + bottom);
		frontline[6].position.x = x + out * spacing;		frontline[6].position.y = y + h - out * (spacing + bottom);
		frontline[7].position.x = x + out * spacing;		frontline[7].position.y = y + out * spacing;

		// floor
		for (int i = 0; i < 4; i++) {
			back[i].position.x = std::floor(back[i].position.x);
			back[i].position.y = std::floor(back[i].position.y);
			front[i].position.x = std::floor(front[i].position.x);
			front[i].position.y = std::floor(front[i].position.y);
		}
		for (int i = 0; i < 8; i++) {
			backline[i].position.x = std::floor(backline[i].position.x);
			backline[i].position.y = std::floor(backline[i].position.y);
			frontline[i].position.x = std::floor(frontline[i].position.x);
			frontline[i].position.y = std::floor(frontline[i].position.y);
		}
	};

	void initTheme() {
		sf::Color col = color(c::outside, 255);
		for (int i = 0; i < 4; i++)
			back[i].color = col;

		col = color(c::inside, 255);
		for (int i = 0; i < 4; i++)
			front[i].color = col;

		col = color(c::outline, 255);
		for (int i = 0; i < 8; i++) {
			backline[i].color = col;
			frontline[i].color = col;
		}
	};

	void draw(sf::RenderTarget& target) {
		target.draw(back, 4, sf::Quads);
		target.draw(front, 4, sf::Quads);
		target.draw(backline, 8, sf::Lines);
		target.draw(frontline, 8, sf::Lines);
	}
};

class Render {
public:
	Render() {}

	sf::RenderTexture texture;
	sf::Sprite sprite;

	void create(unsigned int w, unsigned int h) {
		texture.create(w, h);
		sprite.setTextureRect(sf::IntRect(0, 0, w, h));
	}
};

class Scroll {
public:
	sf::RenderTexture render;
	sf::Sprite sprite;
	sf::Vector2f scroll;
	sf::Vector2f appr;
	sf::Vector2f apprLast;

	Scroll() {
		scroll.x = 0; scroll.y = 0;
		appr.x = 0; appr.y = 0;
		apprLast.x = 0; apprLast.y = 0;
	}

	void create(int w, int h) {
		render.create(w, h);
	}

	void copy(Container &from) {
		sprite.setPosition(from.x, from.y);
		create((int)from.w, (int)from.h);
	}

	void smoothing() {
		appr.x = approach(appr.x, scroll.x, abs(appr.x - scroll.x) / 3.0f);
		appr.y = approach(appr.y, scroll.y, abs(appr.y - scroll.y) / 3.0f);
	}

	void last() {
		apprLast.x = appr.x;
		apprLast.y = appr.y;
	}

	void draw(sf::RenderTarget& target) {
		render.display();
		sprite.setTexture(render.getTexture());
		target.draw(sprite);
	}
};
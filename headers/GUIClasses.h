#pragma once
#pragma warning (disable : 26812)

#include "GUIData.h"

#include <SFML/Graphics.hpp>

#define EMPTY(x) void update() x; void draw(sf::RenderTarget& target) x;
#define EMPTY_I void update(); void draw(sf::RenderTarget& target);

namespace gui {
	class Render {
	public:
		sf::Vector2f pos = { 0, 0 };

		sf::RenderTexture texture;
		sf::Sprite sprite;

		void clear(sf::Color col = sf::Color());
		void create(unsigned int w, unsigned int h);
		void display();
		EMPTY_I
	};

	class Empty {
	public:
		sf::Vector2f pos = { 0, 0 };
		sf::Vector2f size = { 0, 0 };

		void setPos(float x, float y);
		void setPos(sf::Vector2f p);
		void setSize(float w, float h);
		void setSize(sf::Vector2f s);
		void set(float x, float y, float w, float h);
		void set(sf::Vector2f p, sf::Vector2f s);
		void round();

		virtual void update() = NULL;
		virtual void draw(sf::RenderTarget& target) = NULL;
	};

	class Rectangle : public Empty {
	private:
		static const int vertCount = 4;

		sf::Vertex vert[vertCount];
	public:
		Rectangle() {}
		Rectangle(float x, float y, float w, float h);
		Rectangle(sf::Vector2f p, sf::Vector2f s);

		void setColor(sf::Color col);

		EMPTY(override final)
	};

	class RectangleOut : public Empty {
	private:
		static const int vertCount = 5;

		sf::Vertex vert[vertCount];
	public:
		RectangleOut() {};
		RectangleOut(float x, float y, float w, float h);
		RectangleOut(sf::Vector2f p, sf::Vector2f s);

		void setColor(sf::Color col);

		EMPTY(override final)
	};
	
	class ProgressBar : public Empty {
	private:
		Rectangle bg;
		Rectangle val;
		RectangleOut out;
		sf::Sprite shadow;

	public:
		sf::Color* color = NULL;

		float* refValue = NULL;
		float maxValue = 1.f;

		bool vertical = true;

		void setTextures(sf::Texture& tx1);
		void updateValue();

		EMPTY(override final)
	};

	class Checkbox : public Empty {
	private:
		Rectangle bg;
		Rectangle val;
		RectangleOut out;
		sf::Sprite shadow;
	public:
		sf::Color* color = NULL;

		bool* refValue = NULL;

		void setTextures(sf::Texture& tx1);
		void updateValue();
		void check(int mx, int my);

		EMPTY(override final)
	};

	class Button : public Empty {
	private:
		sf::Sprite sprite;
		sf::Sprite highlight;

	public:
		bool checked = false;
		float alpha = 0.f;
		sf::Vector2f defScale;

		sf::Sprite& getSprite() { return sprite; }
		sf::Sprite& getHighlight() { return highlight; }

		void setTextures(sf::Texture& tx1, sf::Texture& tx2);
		bool check(int mx, int my);

		virtual void setScale(float sx, float sy) = NULL;
		virtual void animate() = NULL;

		EMPTY(override final)
	};

	class ButtonBig : public Button {
	public:
		void setScale(float sx, float sy) override final;
		void animate() override final;
	};

	class Graph : public Empty {
	private:
		Rectangle bg;
		Rectangle front;
		RectangleOut bgOut;
		RectangleOut frontOut;
		sf::Sprite shadow;
		sf::Vertex* line;
		sf::Vector2f* points;
		unsigned int pointCount = 0;
	public:
		void setTextures(sf::Texture& tx1);
		void createLine(unsigned int _pointCount);
		void updateLine();
		void setPoint(unsigned int _point, float _x, float _y);
		sf::Vector2f getPoint(unsigned int _point) { return points[_point]; }
		unsigned int getPointCount() { return pointCount; }

		EMPTY(override final)
	};

	class ScrollPanel : public Empty {
	private:
		sf::RenderTexture texture;
		sf::Sprite sprite;

	public:
		// dragging
		bool drag = false;
		sf::Vector2i mousePosLast;
		sf::Vector2i mousePosStart;
		// sliders
		Rectangle sliderX[2];
		Rectangle sliderY[2];
		// scroll
		sf::Vector2f scroll;
		sf::Vector2f scrollApp;
		sf::Vector2f scrollMax;

		int* scrollScale = NULL;

		void display();
		void create();
		void smooth();
		void clear(sf::Color color = sf::Color());
		void check(int mx, int my);
		void clamp();
		sf::RenderTexture& getTexture() { return texture; }

		EMPTY(override final)
	};

	class Widget : public Empty {
	private:
		// rectangles
		Rectangle bg;
		Rectangle front;
		RectangleOut bgOut;
		RectangleOut frontOut;

		// should draw scroll?
		bool scroll = false;

		// drag handles (sprites)
		sf::Sprite sprDragL;
		sf::Sprite sprDragT;

		// widget resizing
		bool dragLeft = false;
		bool dragTop = false;
		sf::Vector2i mousePosLast;
		sf::Vector2i mousePosStart;
	public:
		// scroll
		ScrollPanel* scrollPanel = NULL;

		// manual adjustment for buttons
		float bottom = 5.f;

		// should check for widget resizing?
		bool checkLeft = false;
		bool checkTop = false;

		// buttons
		std::vector<Button*> buttons;

		void addButton(Button* button);
		void check(int mx, int my);
		void handlesSetup();
		void createScrollPanel();
		void setHandlesScale(float sx, float sy);
		sf::Vector2f getInsidePos() { return front.pos; }
		sf::Vector2f getInsideSize() { return front.size; }

		EMPTY(override final)
	};

	class Channels {
	private:
		// ui
		Rectangle* rect = NULL;
		RectangleOut* rectOut = NULL;
		sf::Text* text = NULL;
		
		ProgressBar* volume = NULL;
		Checkbox* muted = NULL;

		// slot count (optimization)
		unsigned int width = 0u, height = 0u;
		unsigned int lastx = 0u, lasty = 0u;
	public:
		ScrollPanel* scrollPanel = NULL; // for scroll state access
		Render renderRect, renderText, renderBar;

		void recalculate(bool rescale = false); // recalculates and allocates required memory for slot caching (optimization)
		void refresh(); // sets pattern slot numbers according to current scroll
		void updateChannels(); // rearranges rects and text
		void check(int mx, int my);

		EMPTY_I
	protected:
		void cacheRects();
		void cacheText();
		void cacheBar();

		const sf::BlendMode textBlendMode = sf::BlendMode(sf::BlendMode::One, sf::BlendMode::DstColor);
	};
}
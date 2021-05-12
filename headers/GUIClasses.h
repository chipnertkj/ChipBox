#ifndef GUICLASSES_H
#define GUICLASSES_H
#pragma warning (disable : 26812)

#include "GUIData.h"

#include <SFML/Graphics.hpp>

namespace gui {
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

		virtual void update();
		virtual void draw(sf::RenderTarget& target);
	};

	class Rectangle : public Empty {
	private:
		static const int vertCount = 4;

		sf::Vertex vert[vertCount];
	public:
		Rectangle();
		Rectangle(float x, float y, float w, float h);

		void setColor(sf::Color col);

		void update() override final;
		inline void draw(sf::RenderTarget& target) override final;
	};

	class RectangleOut : public Empty {
	private:
		static const int vertCount = 5;

		sf::Vertex vert[vertCount];
	public:
		void setColor(sf::Color col);

		void update() override final;
		inline void draw(sf::RenderTarget& target) override final;
	};

	class Button : public Empty {
	private:
		sf::Sprite sprite;
		sf::Sprite highlight;

	public:
		bool checked = false;
		float alpha = 0;
		sf::Vector2f defScale;

		sf::Sprite& getSprite();
		sf::Sprite& getHighlight();

		void setTextures(sf::Texture& tx1, sf::Texture& tx2);
		bool check(int mx, int my);

		virtual void setScale(float sx, float sy);
		virtual void animate();

		virtual void update() override final;
		virtual void draw(sf::RenderTarget& target) override final;
	};

	class ButtonBig : public Button {
	public:
		void setScale(float sx, float sy) override final;
		void animate() override final;
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

		int* scrollScale = 0;

		void display();
		void create();
		void smooth();
		void clear(sf::Color color = sf::Color());
		void check(int mx, int my);
		void clamp();
		sf::RenderTexture& getTexture();

		void update() override final;
		inline void draw(sf::RenderTarget& target) override final;
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
		ScrollPanel* scrollPanel = 0;

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
		sf::Vector2f getInsidePos();
		sf::Vector2f getInsideSize();

		void update() override final;
		void draw(sf::RenderTarget& target) override final;
	};

	class Channels {
	private:
		// ui
		Rectangle* rect = 0;
		RectangleOut* rectOut = 0;
		sf::Text* text = 0;
		// slot count (optimization)
		unsigned int width = 0u;
		unsigned int height = 0u;
	public:
		int scale = cs.patFull; // scroll scale
		ScrollPanel* scrollPanel = 0; // for scroll state access

		void recalculate(); // recalculates and allocates required memory for slot caching (optimization)
		void refreshText(); // sets pattern slot numbers according to current scroll

		void update();
		void draw(sf::RenderTarget& target);
	};

	class Render {
	public:
		sf::RenderTexture texture;
		sf::Sprite sprite;

		void clear(sf::Color col = sf::Color());
		void create(unsigned int w, unsigned int h);
		void display();
		inline void draw(sf::RenderTarget& target);
	};

}

#endif
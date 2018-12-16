#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

class Widget {
public:
	Widget();
	Widget(Widget* parent);
	~Widget();

	virtual void draw() {};

	Widget* getParent() const;
	void addChild(Widget* child);

	virtual bool handleEvent(sf::Event event);

	void takeFocus();
	bool hasFocus() const;

	virtual const sf::Vector2f& getPosition() const = 0;
	virtual void setPosition(const sf::Vector2f& position) = 0;
	virtual const sf::Vector2f getSize() const {
		throw std::exception("getSize() not implemented");
	};

protected:
	Widget* parent;
	std::vector<Widget*> children;
	bool removeChild(const Widget* child);
};


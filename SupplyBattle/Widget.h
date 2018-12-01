#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

class Widget {
public:
	Widget();
	Widget(Widget* parent);
	~Widget();

	Widget* getParent() const;
	virtual bool handleEvent(sf::Event event);
	void takeFocus();
	bool hasFocus() const;
	void addChild(Widget* child);

protected:
	Widget* parent;
	std::vector<Widget*> children;
	bool removeChild(const Widget* child);
};


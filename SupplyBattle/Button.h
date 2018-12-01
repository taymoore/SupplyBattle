#pragma once
#include <Windows.h>
#include <SFML/Graphics.hpp>
#include "Assets.h"
#include "Widget.h"

class Button :
	public Widget {
public:
	Button(const sf::String& string, const sf::Vector2f& pos, const float& height, Widget* parent);
	~Button();

    bool handleEvent(sf::Event event) override;
	void draw();

protected:
	virtual void clicked() {};						// Mouse is pressed & released while mouse is within button
	virtual void pressed() {};						// Button is pressed down
	virtual void released() {};						// Button is released

private:
	sf::Text text;
	sf::RectangleShape background;

	bool isPressed;
};


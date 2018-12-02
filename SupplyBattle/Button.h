#pragma once
#include <Windows.h>
#include <SFML/Graphics.hpp>
#include "Assets.h"
#include "Widget.h"

class Button :
	public Widget {
public:
	Button(const sf::String & string, const float & height, Widget * parent);
	~Button();

    bool handleEvent(sf::Event event) override;
	void draw();

	const sf::Vector2f& getPosition() const override;
	void setPosition(const sf::Vector2f& position) override;

protected:
	virtual void clicked() {};						// Mouse is pressed & released while mouse is within button
	virtual void pressed();						// Button is pressed down
	virtual void released();						// Button is released

private:
	sf::Text text;
	sf::RectangleShape background;
	const float height;

	bool isPressed;

	// Colors
	static const sf::Color textFillColor;
	static const sf::Color backgroundFillColor;
	static const sf::Color backgroundOutlineColor;

	static const sf::Color textFillColorPressed;
	static const sf::Color backgroundFillColorPressed;
	static const sf::Color backgroundOutlineColorPressed;
};

class NewMapButton :
	public Button {
public:
	NewMapButton(Widget* parent);
	void clicked() override;
};


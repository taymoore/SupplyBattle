#pragma once
#include "Widget.h"
#include "Assets.h"

class CheckBox :
	public Widget {
public:
	CheckBox(const sf::String & string, const float & height, Widget * parent);
	~CheckBox();

    bool handleEvent(sf::Event event) override;
	void draw();

	const sf::Vector2f& getPosition() const override;
	void setPosition(const sf::Vector2f& position) override;
	const sf::Vector2f getSize() const override;

protected:
	virtual void clicked() {};

private:
	sf::Text text;
	sf::RectangleShape box;
	const float height;

	bool isPressed;

	// Dimensions
	static const sf::Vector2f textOffset;
	sf::Vector2f size;

	// Colors
	static const sf::Color textFillColor;
	static const sf::Color backgroundFillColor;
	static const sf::Color backgroundOutlineColor;
};

class GlobalViewCheckbox :
	public CheckBox {
public:
	GlobalViewCheckbox(Widget* parent);
private:
	void clicked() override;
};

//class PlayerViewCheckbox :
//	public CheckBox {
//	PlayerViewCheckbox(Widget* parent);
//};

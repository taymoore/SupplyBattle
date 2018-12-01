#pragma once
#include <SFML/Graphics.hpp>
#include "Assets.h"
#include "Widget.h"

class Button :
	public Widget {
public:
	Button(sf::Vector2f& pos, sf::String& string);
	~Button();

	void draw();

private:
	sf::Text string;
	sf::RectangleShape background;

};


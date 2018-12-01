#pragma once
#include <SFML/Graphics.hpp>
#include "Assets.h"
#include "Widget.h"

class Hud;

class Label :
	public Widget {
public:
	Label(sf::String & string, Hud & parent);
	~Label();

private:
	Hud& parent;

	sf::Text string;
	sf::RectangleShape background;
};


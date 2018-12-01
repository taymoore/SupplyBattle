#pragma once
#include <SFML/Graphics.hpp>

class Hud {
public:
	Hud(sf::Vector2f& pos, sf::Vector2f& dim);
	~Hud();

	void draw();

private:
	sf::RectangleShape background;
};


#pragma once
#include <SFML/Graphics.hpp>
#include "Assets.h"

class Tile;

class Unit {
public:
	Unit(Tile& tile);
	~Unit();

	void draw();

private:
	Tile* tile;

	sf::Sprite sprite;
};


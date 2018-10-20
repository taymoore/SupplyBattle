#pragma once
#include <array>
#include <SFML\Graphics.hpp>

class Tile;

class Road {
public:
	Road(Tile& tile1, Tile& tile2);
	~Road();

	bool contains(const Tile& tile) const;

	void draw(sf::RenderWindow& renderWindow);
private:
	std::array<std::reference_wrapper<Tile>, 2> tileList;
};


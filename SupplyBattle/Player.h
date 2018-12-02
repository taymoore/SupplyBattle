#pragma once
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "Tile.h"

class Player {
public:
	Player();
	~Player();

	void draw();

	void assignCity(Tile& tile);
	void setColor(const sf::Color& color);

private:
	struct City {
		City(Tile& tile, const sf::Color& color);
		void draw();
		Tile& tile;
		sf::ConvexShape banner;
	};
	std::unordered_map<Tile*, City> cityList;

	sf::Color color;
};


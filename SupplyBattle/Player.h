#pragma once
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "Tile.h"

class Player {
public:
	Player();
	~Player();
	void assignCity(Tile& tile);

private:
	struct City {
	};
	std::unordered_map<Tile*, City> cityList;
};


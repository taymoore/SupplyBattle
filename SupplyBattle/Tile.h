#pragma once
#include <exception>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Terrain.h"
#include "Assets.h"
#include "Road.h"

class Tile {
public:
	Tile();
//    Tile(const int& x, const int& y, const int& z);
	Tile(const int & a, const int & b, const Terrain::TerrainType & terrainType);
	Tile(const sf::Vector3i& pos, const Terrain::TerrainType & terrainType);
	//Tile& operator=(const Tile&);
	bool operator==(const Tile& other) const;
	bool operator<(const Tile& other) const;

    static sf::Vector2u getPos(const int& x, const int& y, const int& z);
    static sf::Vector2u getPos(const sf::Vector3<int>& pos);
    static sf::Vector3i getPos(const int& a, const int& b);
    static sf::Vector3i getPos(const sf::Vector2<int>& pos);

	static unsigned int getDistance(const Tile& tile1, const Tile& tile2);

	const sf::Vector2u& getPos2() const;
	const sf::Vector3i& getPos3() const;

	void setString(const sf::String& string);
	void draw(sf::RenderWindow& renderWindow);
	sf::CircleShape drawCircle(sf::RenderWindow & renderWindow, const sf::Color & color, const float & radius = 100) const;

	void setAdjacentTiles(std::array<Tile*, 6>& adjacentTiles);
	std::array<Tile*, 6>& getAdjacentTiles();

	const Terrain& getTerrain() const;
	Terrain& getTerrain();
	const unsigned int& getMovementCost() const;
	const unsigned int getMovementCost(const Tile & tile) const;

	void assignRoad(Road& road);

	const sf::Vector2f getPixCentre() const;
	const sf::Vector2f getPixPos() const;

private:
    // Position
    sf::Vector3i pos3;
    sf::Vector2u pos2;
	sf::Vector2f pixPos;

	std::array<Tile*, 6> adjacentTiles;
	Terrain terrain;
	std::vector<std::reference_wrapper<Road>> roadList;

	sf::Text text;
};
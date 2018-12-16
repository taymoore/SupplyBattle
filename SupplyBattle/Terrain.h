#pragma once
#include <SFML\Graphics.hpp>
#include "Assets.h"

class Terrain {
public:
	enum TerrainType {
		NONE,
		GRASSLAND,
		LIGHT_FOREST,
		THICK_FOREST,
		HILLS,
		WOODED_HILLS,
		MOUNTAIN,
		COAST,
		OCEAN,
		TOWN
	};
	Terrain();
	Terrain(const sf::Vector2<float>& pos, const Terrain::TerrainType & terrainType);
	~Terrain();

	void draw(sf::RenderWindow& renderWindow);
    void setTerrainType(const TerrainType& terrianType);
	void setSpriteColor(const sf::Color& color);
	const TerrainType& getTerrainType() const;
	const unsigned int& getMovementCost() const;

private:
	TerrainType terrainType;
	sf::Sprite sprite;
	unsigned int movementCost;
};


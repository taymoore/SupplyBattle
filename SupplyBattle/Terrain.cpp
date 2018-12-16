#include "Terrain.h"

extern Assets assets;

Terrain::Terrain() {
}

Terrain::Terrain(const sf::Vector2<float>& pos, const Terrain::TerrainType& terrainType) :
	terrainType(terrainType),
    sprite(assets.getTerrainTileset()) {
    setTerrainType(terrainType);
	sprite.setPosition(pos);
	sprite.setScale(10, 10);
}

Terrain::~Terrain() {
}

void Terrain::draw(sf::RenderWindow & renderWindow) {
    renderWindow.draw(sprite);
}

void Terrain::setTerrainType(const TerrainType & terrainType) {
    this->terrainType = terrainType;
    unsigned int spriteIndex_X, spriteIndex_Y;
    switch(terrainType) {
        case NONE:
			return;
        case GRASSLAND:
            spriteIndex_X = 0;
            spriteIndex_Y = 0;
			movementCost = 2;
            break;
        case LIGHT_FOREST:
            spriteIndex_X = 1;
            spriteIndex_Y = 0;
			movementCost = 3;
            break;
        case THICK_FOREST:
            spriteIndex_X = 2;
            spriteIndex_Y = 0;
			movementCost = 5;
            break;
        case HILLS:
            spriteIndex_X = 3;
            spriteIndex_Y = 0;
			movementCost = 4;
            break;
        case WOODED_HILLS:
            spriteIndex_X = 4;
            spriteIndex_Y = 0;
			movementCost = 6;
            break;
        case MOUNTAIN:
            spriteIndex_X = 5;
            spriteIndex_Y = 0;
			movementCost = 10;
            break;
        case COAST:
            spriteIndex_X = 6;
            spriteIndex_Y = 0;
			movementCost = 100;
            break;
        case OCEAN:
            spriteIndex_X = 7;
            spriteIndex_Y = 0;
			movementCost = 1000;
            break;
        case TOWN:
            spriteIndex_X = 0;
            spriteIndex_Y = 1;
			movementCost = 1;
            break;
    }
    sf::IntRect spriteRect(spriteIndex_X * 32, spriteIndex_Y * 48 + 2, 32, 48);
    sprite.setTextureRect(spriteRect);
}

void Terrain::setSpriteColor(const sf::Color & color) {
	sprite.setColor(color);
}

const Terrain::TerrainType & Terrain::getTerrainType() const {
	return terrainType;
}

const unsigned int& Terrain::getMovementCost() const {
	return movementCost;
}

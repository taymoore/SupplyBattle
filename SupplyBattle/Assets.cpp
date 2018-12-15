#include "Assets.h"

Assets::Assets() {
    terrainTileset.loadFromFile("assets/tiles/terrain.png");
    unitTileset.loadFromFile("assets/tiles/units.png");
	font.loadFromFile("assets/fonts/system.ttf");
}

Assets::~Assets() {
}

const sf::Texture& Assets::getUnitTileset() const {
    return unitTileset;
}

sf::Texture & Assets::getTerrainTileset() {
    return terrainTileset;
}

const sf::Texture& Assets::getTerrainTileset() const {
    return terrainTileset;
}

const sf::Font & Assets::getFont() const {
	return font;
}

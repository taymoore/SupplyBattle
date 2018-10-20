#include "Assets.h"



Assets::Assets() {
    terrainTileset.loadFromFile("assets/tiles/terrain.png");
	font.loadFromFile("assets/fonts/system.ttf");
}


Assets::~Assets() {
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

#include "Tile.h"
#include "Map.h"

extern Assets assets;
extern Map* map_;

Tile::Tile() :
	road(false) {
}

//Tile::Tile(const int& x, const int& y, const int& z) :
//	Tile(sf::Vector3<int>(x, y, z)) {
//}

Tile::Tile(const int & a, const int & b, const Terrain::TerrainType& terrainType) :
	Tile(sf::Vector3i(Tile::getPos(a, b)), terrainType) {
}

Tile::Tile(const sf::Vector3i& pos, const Terrain::TerrainType& terrainType) :
	pos2(Tile::getPos(pos)),
	pos3(pos),
	pixPos(static_cast<float>(pos2.x * 240), static_cast<float>(pos2.x % 2 ? pos2.y * 280 : pos2.y * 280 + 140)),
	terrain(pixPos, terrainType),
	road(false),
	text("", assets.getFont(), 50) {
	text.setPosition(pixPos + sf::Vector2f(120.0f, 240.0f));
}

bool Tile::operator==(const Tile & other) const {
	if(getPos2() == other.getPos2()) {
		return true;
	} else {
		return false;
	}
}

bool Tile::operator<(const Tile & other) const {
	if(getPos2().y * map_->getMapSize().x + getPos2().x < other.getPos2().y * map_->getMapSize().x + other.getPos2().x) {
		return true;
	} else {
		return false;
	}
}

void Tile::drawTerrain(sf::RenderWindow& renderWindow) {
	terrain.draw(renderWindow);
	renderWindow.draw(text);
}

void Tile::drawRoad(sf::RenderWindow & renderWindow) {
	for(std::array<sf::Vertex, 2>& line : roadGraphic) {
		renderWindow.draw(line.data(), 2, sf::Lines);
	}
}

sf::CircleShape Tile::drawCircle(sf::RenderWindow & renderWindow, const sf::Color & color, const float& radius) const {
	sf::CircleShape circle(radius);
	circle.setPosition(pixPos + sf::Vector2f(60.0f, 180.0f));
	circle.setFillColor(color);
	return circle;
}

void Tile::setAdjacentTiles(std::array<Tile*, 6>& adjacentTiles) {
	this->adjacentTiles = adjacentTiles;
}

std::array<Tile*, 6>& Tile::getAdjacentTiles() {
	return adjacentTiles;
}

sf::Vector2u Tile::getPos(const int & x, const int & y, const int & z) {
    return sf::Vector2u(-z, x + z / 2);
}

sf::Vector2u Tile::getPos(const sf::Vector3<int>& pos) {
    return sf::Vector2u(Tile::getPos(pos.x, pos.y, pos.z));
}

sf::Vector3i Tile::getPos(const int & a, const int & b) {
    return sf::Vector3i(a / 2 + b, (a + 1) / 2 - b, -a);
}

sf::Vector3i Tile::getPos(const sf::Vector2<int>& pos) {
    return sf::Vector3i(Tile::getPos(pos.x, pos.y));
}

unsigned int Tile::getDistance(const Tile & tile1, const Tile & tile2) {
	return std::max(std::max(std::abs(tile1.getPos3().x - tile2.getPos3().x), std::abs(tile1.getPos3().y - tile2.getPos3().y)), std::abs(tile1.getPos3().z - tile2.getPos3().z));
}

const sf::Vector2u& Tile::getPos2() const {
	return pos2;
}

const sf::Vector3i& Tile::getPos3() const {
	return pos3;
}

void Tile::setString(const sf::String & string) {
	text.setString(string);
}

const Terrain & Tile::getTerrain() const {
	return terrain;
}

Terrain & Tile::getTerrain() {
	return terrain;
}

const unsigned int Tile::getMovementCost() const {
	if(hasRoad()) {
		return 1;
	} else {
		return getTerrain().getMovementCost();
	}
}

void Tile::createRoad() {
	road = true;
	updateRoadGraphic();
}

bool Tile::hasRoad() const {
	return road;
}

const sf::Vector2f Tile::getPixCentre() const {
	return pixPos + sf::Vector2f(160, 300);
}

const sf::Vector2f Tile::getPixPos() const {
	return pixPos;
}

void Tile::updateRoadGraphic() {
	roadGraphic.clear();
	if(hasRoad()) {
		for(Tile* adjacentTile : adjacentTiles) {
			if(adjacentTile != nullptr && adjacentTile->hasRoad()) {
				roadGraphic.push_back({adjacentTile->getPixCentre(), getPixCentre()});
			}
		}
		if(roadGraphic.empty()) {
			roadGraphic.push_back({getPixCentre() - sf::Vector2f(-10, -10), getPixCentre() + sf::Vector2f(10, 10)});
		}
	}
}

#pragma once

#include <vector>
#include <map>
#include <SFML/Graphics.hpp>
#include "Map.h"

class DebugView {
public:
	DebugView();
	~DebugView();

	void addCircle(const Tile & tile, const sf::Color & color, const float & radius = 100.0f);
	std::size_t eraseCircle(const Tile& tile);
	void addText(const Tile & tile, const sf::String & string);
	std::size_t eraseText(const Tile& tile);
	void addLine(const Tile& start, const Tile& finish);
	std::size_t eraseLine(const Tile & start, const Tile & finish);
	bool draw();
	void clear();

private:
	std::map<Tile, sf::CircleShape> circleList;
	std::map<Tile, sf::Text> textList;
	std::map<std::pair<Tile, Tile>, sf::VertexBuffer> lineList;
};


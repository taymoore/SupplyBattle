#pragma once
#include <Windows.h>
#include <memory>
#include <array>
#include <vector>
#include <string>
#include <list>
#include <forward_list>
#include <ostream>
#include <sstream>
#include <cstdlib>
#include <random>
#include <ctime>
#include <algorithm>
#include "Assets.h"
#include "Tile.h"
#include "FastNoise.h"
#include "DebugView.h"

class Map {
public:
	Map(sf::RenderWindow& renderWindow, const uint32_t& xSize, const uint32_t& ySize);
	~Map();
	void draw(sf::RenderWindow& renderWindow);
	const sf::Vector2u& getMapSize() const;
	/*
	Tile& getTile(const uint32_t& x, const uint32_t& y, const uint32_t& z);
	*/
    // Returns bool if event is handled
    bool handleEvent(sf::Event sfEvent, sf::RenderWindow& renderWindow);

	enum Direction {
		UP_LEFT,
		UP,
		UP_RIGHT,
		DOWN_RIGHT,
		DOWN,
		DOWN_LEFT
	};

	Tile* getTile(const Tile & tile, const Map::Direction & direction);
	std::vector<Tile*> getPath(Tile& start, Tile& finish);

	void createRoad(Tile& tile);
	void generateRoads();

private:
	// Tiles
	std::vector<Tile> tileList;
	Tile& getTile(const int& x, const int& y, const int& z);
	Tile* getTilePtr(const int& x, const int& y, const int& z);
	const Tile& getTile(const int & x, const int & y, const int & z) const;
	std::array<Tile*, 6> getAdjacentTiles(const sf::Vector3i& pos);

	// Towns
	std::vector<std::reference_wrapper<Tile>> townList;

	// Map
	sf::Vector2<unsigned int> mapSize;

	// View
    sf::View view;
    bool isPanning = false;
    sf::Vector2i oldMousePoint;
    float viewZoom;
    static constexpr const float viewZoomInit = 2.0f;
    void zoomViewAt(sf::Vector2i pixel, sf::RenderWindow& window, float zoom);
    enum PanVerticalDirection {
        VerticalNone,
        Up,
        Down,
    };
    PanVerticalDirection panVerticalDirection;
    enum PanHorizontalDirection {
        HorizontalNone,
        Left,
        Right
    };
    PanHorizontalDirection panHorizontalDirection;
    int panDelayCount;
    static const int panDelay = 5;
    static const int initPanDelay = -30;
    const float panStep = 30.0f;
};
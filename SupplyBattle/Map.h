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
#include <functional>
#include "Assets.h"
#include "Tile.h"
#include "FastNoise.h"
#include "DebugView.h"
#include "Widget.h"
#include "Player.h"

class Map :
	public Widget {
public:
	Map(const sf::Vector2u & mapSize);
	~Map();

	void generateMap(const sf::Vector2u & size, const unsigned int & playerCount);

	void draw(sf::RenderWindow& renderWindow);
	const sf::Vector2u& getMapSize() const;

    bool handleEvent(sf::Event event) override;

	enum Direction : unsigned int {
		NONE = 0,
		UP_LEFT,
		UP,
		UP_RIGHT,
		DOWN_RIGHT,
		DOWN,
		DOWN_LEFT
	};

	// Update
	void update();

	// Widget
	const sf::Vector2f& getPosition() const override;
	void setPosition(const sf::Vector2f& position) override;

	// Tiles
	static unsigned int getDistance(const Tile& tile1, const Tile& tile2);
	Tile* getTile(Tile & tile, const Map::Direction & direction);
	sf::Vector3i getTile(const sf::Vector3i& pos, const Map::Direction & direction);
	Tile& getTileFromMouse();
	Tile* getTile(Tile & tile, const unsigned int & range, std::function<bool(const Tile&)> test);
	std::vector<Tile*> getTiles(Tile & tile, const unsigned int & range, std::function<bool(const Tile&)> test);
	std::vector<Tile*> getPath(Tile& start, Tile& finish, int range = -1);

	// Towns

	// Players
	std::forward_list<Player>& getPlayerList();
	void clearFogOfWar(Tile& tile, const unsigned int& range);
	const Player* const getPlayerView() const;
	void setPlayerView(Player* const player);

private:
	// Tiles
	std::vector<Tile> tileList;
	Tile& getTile(const int& x, const int& y, const int& z);
	Tile& getTile(const int& a, const int& b);
	Tile& getTile(const sf::Vector2f& pos);	// get tile from pixel position
	Tile* getTilePtr(const sf::Vector3i& pos);
	Tile* getTilePtr(const int& x, const int& y, const int& z);
	const Tile& getTile(const int & x, const int & y, const int & z) const;
	std::array<Tile*, 6> getAdjacentTiles(const sf::Vector3i& pos);

	// Improvements
	std::vector<std::reference_wrapper<Tile>> improvementList;

	// Towns
	std::vector<std::reference_wrapper<Tile>> townList;

	// Players
	std::forward_list<Player> playerList;
	Player* playerView;		// Which player's perspective is active

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
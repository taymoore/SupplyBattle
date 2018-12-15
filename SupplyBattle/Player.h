#pragma once
#include <unordered_map>
#include <forward_list>
#include <SFML/Graphics.hpp>
#include "Tile.h"
#include "Unit.h"

class City;

class Player {
public:
	Player();
	~Player();

	void draw();
	void update();

	void assignCity(Tile& tile);
	void setColor(const sf::Color& color);

	struct City {
		City(Player* const player, Tile& tile, const sf::Color& color);
		void draw();
		void update();

		Tile& tile;
		Player* player;

		struct Resources {
			Resources();
			Resources(unsigned int& wood);
			float wood;
		};
		Resources resources;

		struct Banner {
			Banner(const sf::Vector2f& pos, const sf::Color& color);
			void draw();
			void setResources(const Resources& resources);
			sf::ConvexShape banner;
			sf::Text wood;
		};
		Banner banner;

	};
	const std::unordered_map<Tile*, City>& getCityList() const;

private:
	std::unordered_map<Tile*, City> cityList;

	sf::Color color;
	std::forward_list<Unit> unitList;

};


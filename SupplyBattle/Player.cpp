#include "Player.h"

extern sf::RenderWindow* renderWindow_;
extern Assets assets;

Player::Player() {
}

Player::~Player() {
}

void Player::draw() {
	for(auto& city : cityList) {
		city.second.draw();
	}
}

void Player::assignCity(Tile& tile) {
	//std::unordered_map<Tile*, City> cityList;
	if(cityList.find(&tile) == cityList.end()) {
		tile.assignPlayer(*this);
		cityList.emplace(std::make_pair(&tile, City(tile, color)));
	}
}

void Player::setColor(const sf::Color & color) {
	this->color = color;
}

Player::City::City(Tile& tile, const sf::Color& color) :
	tile(tile),
	banner(5) {
	// Draw banner
	banner.setPoint(0, sf::Vector2f( 90.f, -600.f));
	banner.setPoint(1, sf::Vector2f(230.f, -600.f));
	banner.setPoint(2, sf::Vector2f(230.f, -300.f));
	banner.setPoint(3, sf::Vector2f(160.f, -250.f));
	banner.setPoint(4, sf::Vector2f( 90.f, -300.f));
	banner.setPosition(tile.getPixPos() + sf::Vector2f(0.f, 100.f));
	banner.setFillColor(color);
}

void Player::City::draw() {
	renderWindow_->draw(banner);
}


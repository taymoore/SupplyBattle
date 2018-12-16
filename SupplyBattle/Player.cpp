#include "Player.h"
#include "Map.h"

extern sf::RenderWindow* renderWindow_;
extern Assets assets;
extern Map* map_;

Player::Player() :
	strategy(*this) {
}

Player::~Player() {
}

void Player::draw() {
	for(auto unit : unitList) {
		unit.draw();
	}
	for(auto& city : cityList) {
		city.second.draw();
	}
}

void Player::update() {
	// Update cities
	for(std::pair<Tile* const,City>& city : cityList) {
		city.second.update();
	}
	//for(std::unordered_map<Tile*, City>::iterator cityIter = cityList.begin(); cityIter != cityList.end(); ++cityIter) {
	//	cityIter->second.update();
	//}
}

void Player::assignCity(Tile& tile) {
	if(cityList.find(&tile) == cityList.end()) {
		tile.assignPlayer(*this);
		cityList.emplace(std::make_pair(&tile, City(this, tile, color)));
	}
}

void Player::setColor(const sf::Color & color) {
	this->color = color;
}

const std::unordered_map<Tile*, Player::City>& Player::getCityList() const {
	return cityList;
}

void Player::createUnit(Tile& tile) {
	unitList.emplace_front(tile);
	++strategy.armySize[this];
}

Player::City::City(Player * const player, Tile & tile, const sf::Color & color) :
	player(player),
	tile(tile),
	banner(tile.getPixPos(), color) {
}

void Player::City::draw() {
	banner.draw();
}

void Player::City::update() {
	// Increase resources
	resources.wood += 0.010f;
	banner.setResources(resources);

	// Build Units
	if(resources.wood >= 10) {
		player->createUnit(tile);
		resources.wood -= 10;
	}

	map_->clearFogOfWar(tile, 3);
}

Player::City::Resources::Resources() :
	wood(0) {
}

Player::City::Resources::Resources(unsigned int & wood) :
	wood(static_cast<float>(wood)) {
}

Player::City::Banner::Banner(const sf::Vector2f & pos, const sf::Color & color) :
	wood("", assets.getFont(), 50),
	banner(5) {

	// Draw banner
	banner.setPoint(0, sf::Vector2f( 90.f, -600.f));
	banner.setPoint(1, sf::Vector2f(230.f, -600.f));
	banner.setPoint(2, sf::Vector2f(230.f, -300.f));
	banner.setPoint(3, sf::Vector2f(160.f, -250.f));
	banner.setPoint(4, sf::Vector2f( 90.f, -300.f));
	banner.setPosition(pos + sf::Vector2f(0.f, 100.f));
	banner.setFillColor(color);

	// Add text
	wood.setPosition(pos + sf::Vector2f(110.f, -500.f));
}

void Player::City::Banner::draw() {
	renderWindow_->draw(banner);
	renderWindow_->draw(wood);
}

void Player::City::Banner::setResources(const Resources & resources) {
	wood.setString(std::to_string(static_cast<unsigned int>(resources.wood)));
}

Player::Strategy::Strategy(Player & player) :
	player(player),
	focus(SCOUT) {
}


void Player::Strategy::update() {
}

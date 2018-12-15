#include "Unit.h"
#include "Tile.h"

extern Assets assets;
extern sf::RenderWindow* renderWindow_;

Unit::Unit(Tile& tile) :
    sprite(assets.getUnitTileset()),
	tile(&tile) {
	tile.setUnit(this);
    //sprite.setTextureRect(sf::IntRect(32, 50, 32, 48));
	sprite.setPosition(tile.getPixPos());
	sprite.setScale(10, 10);
}

Unit::~Unit() {
}

void Unit::draw() {
    renderWindow_->draw(sprite);
}

#include "Player.h"

Player::Player() {
}

Player::~Player() {
}

void Player::assignCity(Tile& tile) {
	if(cityList.find(&tile) == cityList.end()) {
		tile.assignPlayer(*this);
	}
}

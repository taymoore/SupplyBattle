#include "Button.h"

extern Assets assets;

Button::Button(sf::Vector2f & pos, sf::String & string) :
	string(string, assets.getFont(), 50) {
	background.setPosition(pos);
}

Button::~Button() {
}

void Button::draw() {
}

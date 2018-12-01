#include "Hud.h"

extern sf::RenderWindow* renderWindow_;

Hud* hud_;

Hud::Hud(sf::Vector2f& pos, sf::Vector2f& dim) :
	background(dim) {
	background.setPosition(pos);
	background.setFillColor(sf::Color(0x7F, 0x7F, 0x7F, 0x7F));
	background.setOutlineColor(sf::Color::White);
	hud_ = this;
}

Hud::~Hud() {
}

void Hud::draw() {
	renderWindow_->setView(renderWindow_->getDefaultView());
	renderWindow_->draw(background);
}

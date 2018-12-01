#include "Button.h"

extern sf::RenderWindow* renderWindow_;
extern Assets assets;
extern Widget* focus_;

Button::Button(const sf::String& string, const sf::Vector2f& pos, const float& height, Widget * parent) :
	Widget(parent),
	text(string, assets.getFont(), static_cast<unsigned int>(height * 0.8f)),
	background(sf::Vector2f(text.getLocalBounds().width + height * 0.3f, height)) {
	text.setPosition(pos + sf::Vector2f(height * 0.1f, height * 0.01f));	// Spacing
	text.setFillColor(sf::Color(0x10, 0x10, 0x10));
	background.setPosition(pos);
	background.setFillColor(sf::Color(0xEA, 0xEA, 0xEA));
	background.setOutlineThickness(1.f);
	background.setOutlineColor(sf::Color(0x10, 0x10, 0x10));
}

Button::~Button() {
}

bool Button::handleEvent(sf::Event event) {
	if(event.type == sf::Event::MouseButtonPressed) {
		sf::Vector2i mousePosition = sf::Mouse::getPosition(*renderWindow_);
		if(mousePosition.x > background.getPosition().x && mousePosition.x < background.getPosition().x + background.getSize().x &&
		   mousePosition.y > background.getPosition().y && mousePosition.y < background.getPosition().y + background.getSize().y) {
			if(focus_ != this) {
				focus_ = this;
				pressed();
				background.setFillColor(sf::Color::Blue);
			}
			return true;
		} else {
			background.setFillColor(sf::Color(0xEA, 0xEA, 0xEA));
			focus_ = nullptr;
			return false;
		}
	} else if(event.type == sf::Event::MouseButtonReleased) {
		if(isPressed) {
			sf::Vector2i mousePosition = sf::Mouse::getPosition(*renderWindow_);
			if(mousePosition.x > background.getPosition().x && mousePosition.x < background.getPosition().x + background.getSize().x &&
			   mousePosition.y > background.getPosition().y && mousePosition.y < background.getPosition().y + background.getSize().y) {
				background.setFillColor(sf::Color::Green);
				clicked();
				released();
				isPressed = false;
				focus_ = nullptr;
				return true;
			} else {
				released();
				isPressed = false;
				focus_ = nullptr;
				return true;
			}
		} else {
			return false;
		}
	} else {
		return false;
	}
}

void Button::draw() {
	renderWindow_->draw(background);
	renderWindow_->draw(text);
}

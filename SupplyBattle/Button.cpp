#include "Button.h"
#include "Map.h"

extern Map* map_;
extern sf::RenderWindow* renderWindow_;
extern Assets assets;
extern Widget* focus_;

const sf::Color Button::textFillColor(sf::Color(0x10, 0x10, 0x10));
const sf::Color Button::backgroundFillColor(sf::Color(0xEA, 0xEA, 0xEA));
const sf::Color Button::backgroundOutlineColor(sf::Color(0x10, 0x10, 0x10));

const sf::Color Button::textFillColorPressed(sf::Color(0xEA, 0xEA, 0xEA));
const sf::Color Button::backgroundFillColorPressed(sf::Color(0x40, 0x40, 0x40));
const sf::Color Button::backgroundOutlineColorPressed(sf::Color(0xEA, 0xEA, 0xEA));

Button::Button(const sf::String& string, const float& height, Widget * parent) :
	Widget(parent),
	height(height),
	isPressed(false),
	text(string, assets.getFont(), static_cast<unsigned int>(height * 0.8f)),
	background(sf::Vector2f(text.getLocalBounds().width + height * 0.3f, height)) {
	text.setFillColor(textFillColor);
	background.setFillColor(backgroundFillColor);
	background.setOutlineThickness(1.f);
	background.setOutlineColor(backgroundOutlineColor);
}

Button::~Button() {
}

bool Button::handleEvent(sf::Event event) {
	if(event.type == sf::Event::MouseButtonPressed) {
		if(event.mouseButton.button == sf::Mouse::Left) {
			sf::Vector2i mousePosition = sf::Mouse::getPosition(*renderWindow_);
			if(mousePosition.x > background.getPosition().x && mousePosition.x < background.getPosition().x + background.getSize().x &&
			   mousePosition.y > background.getPosition().y && mousePosition.y < background.getPosition().y + background.getSize().y) {
				isPressed = true;
				focus_ = this;
				pressed();
				return true;
			} else {
				return false;
			}
		} else {
			return false;
		}
	} else if(event.type == sf::Event::MouseButtonReleased) {
		if(event.mouseButton.button == sf::Mouse::Left) {
			if(isPressed) {
				sf::Vector2i mousePosition = sf::Mouse::getPosition(*renderWindow_);
				if(mousePosition.x > background.getPosition().x && mousePosition.x < background.getPosition().x + background.getSize().x &&
				   mousePosition.y > background.getPosition().y && mousePosition.y < background.getPosition().y + background.getSize().y) {
					background.setFillColor(sf::Color::Green);
					clicked();
					released();
					isPressed = false;
					return true;
				} else {
					released();
					isPressed = false;
					return true;
				}
			} else {
				return false;
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

const sf::Vector2f & Button::getPosition() const {
	return background.getPosition();
}

void Button::setPosition(const sf::Vector2f & position) {
	text.setPosition(position + sf::Vector2f(height * 0.1f, height * 0.01f));	// Spacing
	background.setPosition(position);
}

const sf::Vector2f Button::getSize() const {
	return background.getSize();
}

void Button::pressed() {
	text.setFillColor(textFillColorPressed);
	background.setFillColor(backgroundFillColorPressed);
	background.setOutlineColor(backgroundOutlineColorPressed);
	std::wostringstream os_;
	os_ << "\n" << "down";
	OutputDebugString(os_.str().c_str());
}

void Button::released() {
	text.setFillColor(textFillColor);
	background.setFillColor(backgroundFillColor);
	background.setOutlineColor(backgroundOutlineColor);
	std::wostringstream os_;
	os_ << "\n" << "up";
	OutputDebugString(os_.str().c_str());
}

NewMapButton::NewMapButton(Widget * parent) :
	Button("New Map", 20.f, parent) {
}

void NewMapButton::clicked() {
	map_->generateMap(map_->getMapSize(), 2);
	std::wostringstream os_;
	os_ << "\n" << "pressed";
	OutputDebugString(os_.str().c_str());
}


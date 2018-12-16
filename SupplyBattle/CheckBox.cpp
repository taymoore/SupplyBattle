#include "CheckBox.h"
#include "Map.h"

extern Map* map_;
extern Assets assets;
extern sf::RenderWindow* renderWindow_;
extern Widget* focus_;

const sf::Vector2f CheckBox::textOffset(10.f, 0.f);

const sf::Color CheckBox::textFillColor(sf::Color(0x10, 0x10, 0x10));
const sf::Color CheckBox::backgroundFillColor(sf::Color(0xFA, 0xFA, 0xFA));
const sf::Color CheckBox::backgroundOutlineColor(sf::Color(0x10, 0x10, 0x10));

CheckBox::CheckBox(const sf::String & string, const float & height, Widget * parent) :
	Widget(parent),
	text(string, assets.getFont(), static_cast<unsigned int>(height * 0.8f)),
	box(sf::Vector2f(20.f, 20.f)),
	height(height),
	isPressed(false) {
	text.setFillColor(textFillColor);
	box.setFillColor(backgroundFillColor);
	box.setOutlineThickness(2.f);
	box.setOutlineColor(backgroundOutlineColor);
}

CheckBox::~CheckBox() {
}

bool CheckBox::handleEvent(sf::Event event) {
	if(event.type == sf::Event::MouseButtonPressed) {
		if(event.mouseButton.button == sf::Mouse::Left) {
			sf::Vector2i mousePosition = sf::Mouse::getPosition(*renderWindow_);
			if(mousePosition.x > box.getPosition().x && mousePosition.x < box.getPosition().x + box.getSize().x &&
			   mousePosition.y > box.getPosition().y && mousePosition.y < box.getPosition().y + box.getSize().y) {
				isPressed = true;
				focus_ = this;
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
				if(mousePosition.x > box.getPosition().x && mousePosition.x < box.getPosition().x + box.getSize().x &&
				   mousePosition.y > box.getPosition().y && mousePosition.y < box.getPosition().y + box.getSize().y) {
					box.setFillColor(sf::Color::Green);
					clicked();
					isPressed = false;
					return true;
				} else {
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

void CheckBox::draw() {
	renderWindow_->draw(box);
	renderWindow_->draw(text);
}

const sf::Vector2f & CheckBox::getPosition() const {
	return box.getPosition();
}

void CheckBox::setPosition(const sf::Vector2f & position) {
	box.setPosition(position);
	text.setPosition(sf::Vector2f(box.getSize().x, 0.f) + position + textOffset);
}

const sf::Vector2f CheckBox::getSize() const {
	return sf::Vector2f(box.getSize().x + textOffset.x + text.getLocalBounds().width, height);
}

GlobalViewCheckbox::GlobalViewCheckbox(Widget * parent) :
 CheckBox("Global", 20.f, parent) {
}

void GlobalViewCheckbox::clicked() {
	if(map_->getPlayerView() == nullptr) {
		map_->setPlayerView(&map_->getPlayerList().front());
	} else {
		map_->setPlayerView(nullptr);
	}
}

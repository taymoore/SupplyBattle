#include "Hud.h"

extern sf::RenderWindow* renderWindow_;
extern Assets assets;
extern std::forward_list<Hud> hudList;
extern Widget* focus_;

std::vector<Hud*> hud_;

const sf::Vector2f Hud::verticalListPositionOffset(5.f, 45.f);

const sf::Color Hud::backgroundFillColor(sf::Color(0xFA, 0xFA, 0xFA));
const sf::Color Hud::backgroundOutlineColor(sf::Color(0x10, 0x10, 0x10));
const sf::Color Hud::backgroundFillColorFocused(sf::Color(0xFA, 0xFA, 0xFA));
const sf::Color Hud::backgroundOutlineColorFocused(sf::Color(0x10, 0xA0, 0x10));

Hud::Hud(const sf::String& title, const sf::Vector2f& position, const sf::Vector2f& size, Widget* parent) :
	Widget(parent),
	verticalList(*this),
	background(size),
	newMapButton(&verticalList),
	globalViewCheckBox(&verticalList),
	titlebar(title, position, sf::Vector2f(size.x, 50.f), *this) {	// Height of titlebar
	parent->addChild(this);
	addChild(&verticalList);
	verticalList.addChild(&newMapButton);
	verticalList.addChild(&globalViewCheckBox);
	setPosition(position);
	background.setFillColor(backgroundFillColor);
	background.setOutlineThickness(3.f);
	background.setOutlineColor(backgroundOutlineColor);
	hud_.push_back(this);
}

Hud::~Hud() {
}

bool Hud::handleEvent(sf::Event event) {
	if(event.type == sf::Event::MouseButtonPressed) {
		// If mouse is within hud
		sf::Vector2i mousePosition = sf::Mouse::getPosition(*renderWindow_);
		if(mousePosition.x > background.getPosition().x && mousePosition.x < background.getPosition().x + background.getSize().x &&
		   mousePosition.y > background.getPosition().y - 50.f && mousePosition.y < background.getPosition().y + background.getSize().y) {
			// If hud doesn't have focus
			if(!hasFocus()) {
				takeFocus();
				// Color
				background.setFillColor(backgroundFillColorFocused);
				background.setOutlineColor(backgroundOutlineColorFocused);
				// Move hud to front of list
				if(&*hudList.begin() != this) {
					for(std::forward_list<Hud>::const_iterator prevIter = hudList.begin(); prevIter != hudList.end(); prevIter = std::next(prevIter)) {
						if(&*std::next(prevIter) == this) {
							hudList.splice_after(hudList.before_begin(), hudList, prevIter, std::next(prevIter));
							break;
						}
					}
				}
			}
			// Pass to titlebar
			if(titlebar.handleEvent(event)) {
				return true;
			}
			// Pass to children
			for(Widget* child : children) {
				if(child->handleEvent(event)) {
					break;
				}
			}
			return true;
		// Else mouse is not within hud
		} else {
			//if(hasFocus()) {
				// Color
				background.setFillColor(backgroundFillColor);
				background.setOutlineColor(backgroundOutlineColor);
			//}
			return false;
		}
	} else {
		return false;
	}
}

void Hud::draw() {
	renderWindow_->setView(renderWindow_->getDefaultView());
	titlebar.draw();
	renderWindow_->draw(background);
	verticalList.draw();
}

const sf::Vector2f& Hud::getPosition() const {
	return titlebar.getPosition();
}

void Hud::setPosition(const sf::Vector2f& position) {
	titlebar.setPosition(position);
	background.setPosition(position + sf::Vector2f(0.f, 40.f));
	verticalList.setPosition(position + verticalListPositionOffset);
}

const std::string Hud::to_string(const float & val, const unsigned int & precision) {
	std::ostringstream out;
	out.precision(precision);
	out << val;
	return out.str();
}

const sf::Color Hud::Titlebar::backgroundFillColor(sf::Color(0xF0, 0xF0, 0xF0));
const sf::Color Hud::Titlebar::backgroundOutlineColor(sf::Color(0x20, 0x20, 0x20));
const sf::Color Hud::Titlebar::backgroundFillColorFocused(sf::Color(0xF0, 0xF0, 0xF0));
const sf::Color Hud::Titlebar::backgroundOutlineColorFocused(sf::Color(0x20, 0xA0, 0x20));

Hud::Titlebar::Titlebar(const sf::String& string, const sf::Vector2f& pos, const sf::Vector2f& size, Hud& parent) :
	Widget(parent),
	hud(parent),
	size(size),
	text(string, assets.getFont(), static_cast<unsigned int>(size.y * 0.4f)),
	background(size) {
	text.setFillColor(sf::Color(0x10, 0x10, 0x10));
	background.setFillColor(backgroundFillColor);
	background.setOutlineThickness(3.f);
	background.setOutlineColor(backgroundOutlineColor);
}

bool Hud::Titlebar::handleEvent(sf::Event event) {
	static bool panning = false;
	static sf::Vector2i oldMousePos;
	if(event.type == sf::Event::MouseButtonPressed) {
		if(event.mouseButton.button == sf::Mouse::Button::Left) {
			sf::Vector2i mousePosition = sf::Mouse::getPosition(*renderWindow_);
			if(mousePosition.x > background.getPosition().x && mousePosition.x < background.getPosition().x + background.getSize().x &&
			   mousePosition.y > background.getPosition().y && mousePosition.y < background.getPosition().y + background.getSize().y) {
				panning = true;
				oldMousePos = mousePosition;
				takeFocus();
				background.setFillColor(backgroundFillColorFocused);
				background.setOutlineColor(backgroundOutlineColorFocused);
				return true;
			} else {
				return false;
			}
		} else {
			return false;
		}
	} else if(event.type == sf::Event::MouseButtonReleased) {
		if(event.mouseButton.button == sf::Mouse::Button::Left) {
			panning = false;
			background.setFillColor(backgroundFillColor);
			background.setOutlineColor(backgroundOutlineColor);
			return false;
		} else {
			return false;
		}
	} else if(event.type == sf::Event::MouseMoved) {
		if(panning) {
			hud.setPosition(hud.getPosition() + static_cast<sf::Vector2f>(sf::Mouse::getPosition(*renderWindow_) - oldMousePos));
			oldMousePos = sf::Mouse::getPosition(*renderWindow_);
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}

void Hud::Titlebar::draw() {
	renderWindow_->draw(background);
	renderWindow_->draw(text);
}

const sf::Vector2f & Hud::Titlebar::getPosition() const {
	return background.getPosition();
}

void Hud::Titlebar::setPosition(const sf::Vector2f & position) {
	background.setPosition(position);
	text.setPosition(position + sf::Vector2f(size.x * 0.01f, size.y * 0.1f));	// Spacing
}

void Hud::VerticalList::draw() {
	for(Widget* widget : children) {
		widget->draw();
	}
}

const sf::Vector2f & Hud::VerticalList::getPosition() const {
	return position;
}

void Hud::VerticalList::setPosition(const sf::Vector2f & position) {
	this->position = position;
	sf::Vector2f widgetPos(position);
	for(Widget* widget : children) {
		widget->setPosition(widgetPos);
		widgetPos.y += widget->getSize().y + 10.f;
	}
}

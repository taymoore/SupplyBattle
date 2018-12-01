#include "Widget.h"

extern Widget* focus_;

Widget::Widget() :
	parent(nullptr) {
}

Widget::Widget(Widget* parent) :
	parent(parent) {
}

Widget::~Widget() {
}

Widget* Widget::getParent() const {
	return parent;
}

bool Widget::handleEvent(sf::Event event) {
	for(Widget* widget : children) {
		if(widget->handleEvent(event)) {
			return true;
		}
	}
	return false;
}

void Widget::takeFocus() {
	focus_ = this;
}

bool Widget::hasFocus() const {
	return focus_ == this;
}

bool Widget::removeChild(const Widget * child) {
	for(std::vector<Widget*>::iterator widget = children.begin(); widget != children.end(); ++widget) {
		if(*widget == child) {
			children.erase(widget);
			return true;
		}
	}
	return false;
}

void Widget::addChild(Widget * child) {
	children.push_back(child);
}

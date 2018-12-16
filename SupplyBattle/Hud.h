#pragma once
#include <Windows.h>
#include <sstream>
#include <algorithm>
#include <forward_list>
#include <SFML/Graphics.hpp>
#include "Assets.h"
#include "Widget.h"
#include "Button.h"
#include "CheckBox.h"

class Hud :
	public Widget {
public:
	Hud(const sf::String & title, const sf::Vector2f & pos, const sf::Vector2f & size, Widget * parent);
	~Hud();

    bool handleEvent(sf::Event event) override;
	void draw();

	const sf::Vector2f& getPosition() const override;
	void setPosition(const sf::Vector2f& position) override;

	static const std::string to_string(const float& val, const unsigned int& precision);

private:
	struct Titlebar :
		public Widget {
		Titlebar(const sf::String & title, const sf::Vector2f & pos, const sf::Vector2f& size, Hud & parent);
		bool handleEvent(sf::Event event);
		void draw();
		const sf::Vector2f& getPosition() const override;
		void setPosition(const sf::Vector2f& position) override;
		Hud& hud;
		sf::Text text;
		sf::RectangleShape background;

		const sf::Vector2f size;

		static const sf::Color backgroundFillColor;
		static const sf::Color backgroundOutlineColor;
		static const sf::Color backgroundFillColorFocused;
		static const sf::Color backgroundOutlineColorFocused;
	};
	Titlebar titlebar;

	struct VerticalList :
		public Widget {
		VerticalList(Widget& parent) :
			Widget(parent) {
		}
		void draw() override;
		const sf::Vector2f& getPosition() const override;
		void setPosition(const sf::Vector2f& position) override;
		sf::Vector2f position;

	};
	VerticalList verticalList;
	static const sf::Vector2f verticalListPositionOffset;

	NewMapButton newMapButton;
	GlobalViewCheckbox globalViewCheckBox;
	//std::vector<CheckBox> playerViewCheckboxList;

	sf::RectangleShape background;

	// Consts
	static const sf::Color backgroundFillColor;
	static const sf::Color backgroundOutlineColor;
	static const sf::Color backgroundFillColorFocused;
	static const sf::Color backgroundOutlineColorFocused;
};


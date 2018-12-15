#include <Windows.h>
#include <SFML/Graphics.hpp>
#include "Map.h"
#include "Assets.h"
#include "Hud.h"

Assets assets;
sf::RenderWindow* renderWindow_;
std::forward_list<Hud> hudList;
sf::View hudView;
Widget* focus_;

extern DebugView debugView;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR    lpCmdLine, _In_ int       nCmdShow) {
    sf::VideoMode currentVideoMode = sf::VideoMode::getDesktopMode();

	//sf::ContextSettings settings;
	//settings.antialiasingLevel = 80;
	sf::RenderWindow renderWindow(currentVideoMode, "Supply Battle");// , sf::Style::Default, settings);
	renderWindow_ = &renderWindow;

	// Create map
	Map map(sf::Vector2u(50, 50));
	focus_ = &map;

	// Create Hud
	hudView.reset(sf::FloatRect(0.f, 0.f, static_cast<float>(renderWindow_->getSize().x), static_cast<float>(renderWindow_->getSize().y)));
	hudList.emplace_front("Debug", sf::Vector2f(10,10), sf::Vector2f(200, 30), &map);

	while(renderWindow.isOpen()) {
		// Read
		sf::Event event;
        while(renderWindow.pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                renderWindow.close();
			} else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                renderWindow.close();
            }
			Widget* focus = focus_;
			while(focus != nullptr ? !focus->handleEvent(event) : false) {
				focus = focus->getParent();
			}
        }
		// Update
		map.update();

		// Draw
		renderWindow.clear();
        map.draw(renderWindow);
		debugView.draw();
		for(Hud& hud : hudList) {
			hud.draw();
		}
		renderWindow.display();
    }

	return 0;
}

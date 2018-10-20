#include <Windows.h>
#include <SFML/Graphics.hpp>
#include "Map.h"
#include "Assets.h"

Assets assets;
sf::RenderWindow* renderWindow_;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR    lpCmdLine, _In_ int       nCmdShow) {
    sf::VideoMode currentVideoMode = sf::VideoMode::getDesktopMode();

	//sf::ContextSettings settings;
	//settings.antialiasingLevel = 80;
	sf::RenderWindow renderWindow(currentVideoMode, "Supply Battle");// , sf::Style::Default, settings);
	renderWindow_ = &renderWindow;

	Map map(renderWindow, 50, 50);
	map.generateRoads();



	while(renderWindow.isOpen()) {
		sf::Event event;
        while(renderWindow.pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                renderWindow.close();
			} else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                renderWindow.close();
            }
            // Pass event to arena
            map.handleEvent(event, renderWindow);
        }
		renderWindow.clear();
        map.draw(renderWindow);
		renderWindow.display();
    }

	return 0;
}

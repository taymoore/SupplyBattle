#include <Windows.h>
#include <SFML/Graphics.hpp>
#include "Map.h"
#include "Assets.h"
#include "Hud.h"

Assets assets;
sf::RenderWindow* renderWindow_;
std::forward_list<Hud> hudList;
Widget* focus_;

extern DebugView debugView;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR    lpCmdLine, _In_ int       nCmdShow) {
    sf::VideoMode currentVideoMode = sf::VideoMode::getDesktopMode();

	//sf::ContextSettings settings;
	//settings.antialiasingLevel = 80;
	sf::RenderWindow renderWindow(currentVideoMode, "Supply Battle");// , sf::Style::Default, settings);
	renderWindow_ = &renderWindow;

	// Create map
	Map map(renderWindow, 5, 5);
	focus_ = &map;
	map.generateRoads();

	// Create Hud
	hudList.emplace_front("Title", sf::Vector2f(500,50), sf::Vector2f(200, 500), &map);

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
			//if(focus_ != nullptr ? focus_->handleEvent(event) : false) {
			//	continue;
			//}
			//if(mouseFocus_ != nullptr ? mouseFocus_->handleEvent(event) : false) {
			//	continue;
			//}
			//if(selectedFocus_ != nullptr ? selectedFocus_->handleEvent(event) : false) {
			//	continue;
			//}
			//bool handled = false;
			//for(std::forward_list<Hud>::iterator hud = hudList.begin(); hud != hudList.end(); ++hud) {
			//	if(hud->handleEvent(event)) {
			//		handled = true;
			//	}
			//}
			//if(!handled) {
			//	handled = map.handleEvent(event, renderWindow);
			//}
        }

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

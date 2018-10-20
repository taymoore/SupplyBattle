#include "DebugView.h"

extern sf::RenderWindow* renderWindow_;
extern Map* map_;
extern Assets assets;

DebugView::DebugView() {
}


DebugView::~DebugView() {
}

//void DebugView::addObject(const sf::CircleShape& object) {
//	//circleList.emplace_back(object);
//}

void DebugView::addCircle(const Tile& tile, const sf::Color& color, const float& radius) {
	// If circle doesn't exist
	if(circleList.find(tile) == circleList.end()) {
		circleList.emplace(std::make_pair(tile, sf::CircleShape(radius)));
		circleList.at(tile).setFillColor(color);
		circleList.at(tile).setPosition(tile.getPixPos() + sf::Vector2f(160.0f - radius, 280.0f - radius));
	}
	// Else circle exists
	else {
		circleList.at(tile).setRadius(radius);
		circleList.at(tile).setFillColor(color);
	}
}

std::size_t DebugView::eraseCircle(const Tile & tile) {
	return circleList.erase(tile);
}

void DebugView::addText(const Tile& tile, const sf::String& string) {
	if(textList.find(tile) == textList.end()) {
		textList.emplace(std::make_pair(tile, sf::Text(string, assets.getFont(), 50)));
		textList.at(tile).setPosition(tile.getPixCentre() + sf::Vector2f(-60.0f, -60.0f));
	} else {
		textList.at(tile).setString(string);
	}
}

std::size_t DebugView::eraseText(const Tile & tile) {
	return textList.erase(tile);
}

void DebugView::addLine(const Tile& start, const Tile& finish) {
	if(lineList.find(std::make_pair(start, finish)) == lineList.end()) {
		sf::VertexBuffer vertexBuffer(sf::Lines, sf::VertexBuffer::Usage::Dynamic);
		sf::Vertex vertexArray[2] = {start.getPixCentre(), finish.getPixCentre()};
		vertexBuffer.create(2);
		vertexBuffer.update(vertexArray);
		lineList.emplace(std::make_pair(std::make_pair(start, finish), vertexBuffer));
	}
}

std::size_t DebugView::eraseLine(const Tile& start, const Tile& finish) {
	return lineList.erase(std::make_pair(start, finish));
}

bool DebugView::draw() {
	bool loop = true;
	bool ret = false;
	while(loop) {
		sf::Event event;
		renderWindow_->clear();
		map_->draw(*renderWindow_);
		for(auto objectIter : lineList) {
			renderWindow_->draw(objectIter.second);
		}
		for(auto objectIter : circleList) {
			renderWindow_->draw(objectIter.second);
		}
		for(auto objectIter : textList) {
			renderWindow_->draw(objectIter.second);
		}
		renderWindow_->display();
		while(renderWindow_->pollEvent(event)) {
			if(event.type == sf::Event::Closed) {
				ret = true;
				loop = false;
			} else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
				ret = true;
				loop = false;
			} else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
				ret = false;
				loop = false;
			}
			map_->handleEvent(event, *renderWindow_);
		}
	}
	return ret;
}

void DebugView::clear() {
	circleList.clear();
	textList.clear();
	lineList.clear();
}

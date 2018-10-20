#include "Road.h"
#include "Tile.h"

Road::Road(Tile& tile1, Tile& tile2) :
	tileList{tile1, tile2} {
}

Road::~Road() {
}

bool Road::contains(const Tile & tile) const {
	if(std::find_if(tileList.begin(), tileList.end(), [tile](const std::reference_wrapper<Tile>& other)->bool {return other.get() == tile; }) != tileList.end()) {
		return true;
	} else {
		return false;
	}
}

void Road::draw(sf::RenderWindow & renderWindow) {
	sf::Vertex line[2];
	line[0].position = tileList.at(0).get().getPixCentre();
	line[1].position = tileList.at(1).get().getPixCentre();
	renderWindow.draw(line, 2, sf::Lines);
}

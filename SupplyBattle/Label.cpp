#include "Label.h"
#include "Hud.h"

extern Assets assets;

Label::Label(sf::String& string, Hud& parent) :
	parent(parent),
	string(string, assets.getFont(), 20) {
}


Label::~Label() {
}

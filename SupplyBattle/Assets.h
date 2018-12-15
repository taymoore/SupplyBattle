#pragma once
#include <SFML\Graphics.hpp>
class Assets {
public:
    Assets();
    ~Assets();
	const sf::Texture & getUnitTileset() const;
    sf::Texture& getTerrainTileset();
    const sf::Texture& getTerrainTileset() const;
	const sf::Font& getFont() const;
private:
    sf::Texture terrainTileset;
    sf::Texture unitTileset;
	sf::Font font;
};


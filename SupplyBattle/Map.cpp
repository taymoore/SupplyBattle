#include "Map.h"

extern sf::RenderWindow* renderWindow_;

Map* map_;

DebugView debugView;

Map::Map(sf::RenderWindow& renderWindow, const uint32_t& xSize, const uint32_t& ySize) :
	tileList(xSize * ySize),
	viewZoom(viewZoomInit),
	panVerticalDirection(PanVerticalDirection::VerticalNone),
	panHorizontalDirection(PanHorizontalDirection::HorizontalNone),
	mapSize(xSize, ySize) {

	// Generate Map
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
	FastNoise noiseHeight(std::rand());
	noiseHeight.SetNoiseType(FastNoise::Perlin);
	noiseHeight.SetFrequency(0.05f);
	FastNoise noiseVegetation(std::rand());
	noiseVegetation.SetNoiseType(FastNoise::Perlin);
	noiseVegetation.SetFrequency(0.22f);
	// Map of terrain [Height][Vegetation]
	const Terrain::TerrainType terrainLegend[6][5]{
		{Terrain::TerrainType::OCEAN,		Terrain::TerrainType::OCEAN,		Terrain::TerrainType::OCEAN,		Terrain::TerrainType::OCEAN,		Terrain::TerrainType::COAST},
		{Terrain::TerrainType::COAST,		Terrain::TerrainType::COAST,		Terrain::TerrainType::COAST,		Terrain::TerrainType::COAST,		Terrain::TerrainType::COAST},
		{Terrain::TerrainType::GRASSLAND,	Terrain::TerrainType::GRASSLAND,	Terrain::TerrainType::GRASSLAND,	Terrain::TerrainType::LIGHT_FOREST, Terrain::TerrainType::THICK_FOREST},
		{Terrain::TerrainType::GRASSLAND,	Terrain::TerrainType::GRASSLAND,	Terrain::TerrainType::GRASSLAND,	Terrain::TerrainType::WOODED_HILLS, Terrain::TerrainType::WOODED_HILLS},
		{Terrain::TerrainType::GRASSLAND,	Terrain::TerrainType::GRASSLAND,	Terrain::TerrainType::WOODED_HILLS,	Terrain::TerrainType::MOUNTAIN,		Terrain::TerrainType::MOUNTAIN},
		{Terrain::TerrainType::HILLS,		Terrain::TerrainType::HILLS,		Terrain::TerrainType::WOODED_HILLS,	Terrain::TerrainType::MOUNTAIN,		Terrain::TerrainType::MOUNTAIN}
	};
	for(unsigned int y = 0; y < ySize; ++y) {
		for(unsigned int x = 0; x < xSize; ++x) {
			int heightVal = static_cast<int>(noiseHeight.GetNoise(static_cast<float>(x + 1576), static_cast<float>(y + 735)) * 10.f) + 2;
			if(heightVal < 0) {
				heightVal = 0;
			} else if(heightVal > 5) {
				heightVal = 5;
			}
			int vegVal = static_cast<int>(noiseVegetation.GetNoise(static_cast<float>(x + 1576), static_cast<float>(y + 735)) * 10.f) + 2;
			if(vegVal < 0) {
				vegVal = 0;
			} else if(vegVal > 4) {
				vegVal = 4;
			}
			Terrain::TerrainType terrainType = terrainLegend[heightVal][vegVal];
			tileList.at(x + y * xSize) = Tile(x, y, terrainType);
			//tileList.at(x + y * xSize).setString(std::to_string(heightVal));
		}
	}
	// Determine adjacent tiles
	for(std::vector<Tile>::iterator tileIter = tileList.begin(); tileIter != tileList.end(); ++tileIter) {
		tileIter->setAdjacentTiles(getAdjacentTiles(tileIter->getPos3()));
	}
	// Generate cities
	FastNoise cityNoise(std::rand());
	cityNoise.SetNoiseType(FastNoise::Perlin);
	cityNoise.SetFrequency(0.08f);
	// For each tile on the map
	for(std::vector<Tile>::iterator tileTracer = tileList.begin(); tileTracer != tileList.end(); ++tileTracer) {
		// If tile is a candidate for a city
		if(tileTracer->getTerrain().getTerrainType() == Terrain::TerrainType::GRASSLAND) {
			// Evaluate candidancy
			float cityValue = cityNoise.GetNoise(static_cast<float>(tileTracer->getPos2().x + 1576), static_cast<float>(tileTracer->getPos2().y + 735));
			//tileTracer->setString(std::to_string(cityValue));
			bool isCandidate = true;
			// Compare with adjacent values
			std::array<Tile*, 6> adjacentTileList = getAdjacentTiles(tileTracer->getPos3());
			for(std::array<Tile*, 6>::iterator adjacentTileIter = adjacentTileList.begin(); adjacentTileIter != adjacentTileList.end(); ++adjacentTileIter) {
				if(*adjacentTileIter == nullptr) {
					continue;
				}
				if(cityNoise.GetNoise(static_cast<float>((*adjacentTileIter)->getPos2().x + 1576), static_cast<float>((*adjacentTileIter)->getPos2().y + 735)) >= cityValue) {
					isCandidate = false;
					break;
				}
			}
			if(isCandidate) {
				tileTracer->getTerrain().setTerrainType(Terrain::TerrainType::TOWN);
				townList.push_back(*tileTracer);
			}
		}
	}
	// Generate Players
	//const unsigned int playerCount = 2;
	//enum Quadrant : unsigned int {
	//	TOP_LEFT,
	//	TOP_RIGHT,
	//	BOTTOM_LEFT,
	//	BOTTOM_RIGHT
	//};
	//bool quadrantOccupied[4] = { 0 };
	//for (unsigned int playerCountTracer = 0; playerCountTracer < playerCount; ++playerCountTracer) {
	//	Quadrant selectedQuadrant;
	//	do {
	//		selectedQuadrant = static_cast<Quadrant>(std::rand() % 4);
	//	} while (!quadrantOccupied[selectedQuadrant]);
	//	quadrantOccupied[selectedQuadrant] = true;
	//}

//	view.zoom(3.f);	// Doesn't work since pan rate is not correct
    renderWindow.setView(view);

	map_ = this;
}

void Map::generateRoads() {

	// For all town pairs, sort by distance
	std::multimap<unsigned int, std::pair<std::reference_wrapper<Tile>, std::reference_wrapper<Tile>>> townDistList;
	for(std::vector<std::reference_wrapper<Tile>>::iterator townStartIter = townList.begin(); townStartIter != townList.end(); ++townStartIter) {
		std::vector<std::reference_wrapper<Tile>>::iterator townDestIter = townStartIter;
		while(++townDestIter != townList.end()) {
			townDistList.emplace(Tile::getDistance(townStartIter->get(), townDestIter->get()), std::make_pair(std::reference_wrapper<Tile>(townStartIter->get()), std::reference_wrapper<Tile>(townDestIter->get())));
		}
	}
	// Generate Roads
	for(auto& townPair : townDistList) {
		//debugView.addCircle(townPair.second.first.get(), sf::Color(sf::Color::Green));
		//debugView.addCircle(townPair.second.second.get(), sf::Color(sf::Color::Red));
		//if(debugView.render()) {
		//	return;
		//}
		std::vector<Tile*> path = getPath(townPair.second.first.get(), townPair.second.second.get());
		//debugView.clear();
		if(path.empty()) {
			continue;
		}
		for (Tile* pathIter : path) {
			createRoad(*pathIter);
		}
	}
}

const sf::Vector2f& Map::getPosition() const {
	static sf::Vector2f origin(0.f, 0.f);
	return origin;
}

void Map::setPosition(const sf::Vector2f & position) {
}

Map::~Map() {
}

void Map::draw(sf::RenderWindow& renderWindow) {
    renderWindow.setView(view);
		//// Select tile
		debugView.clear();
		debugView.addCircle(getTile(renderWindow.mapPixelToCoords(sf::Mouse::getPosition(renderWindow))), sf::Color::Green);
		debugView.addText(getTile(renderWindow.mapPixelToCoords(sf::Mouse::getPosition(renderWindow))), "hi");
	// Draw Terrain
	for(unsigned int y = 0; y < mapSize.y; ++y) {
		// Draw top-row
		for(unsigned int x = 1; x < mapSize.x; x += 2) {
			tileList.at(x + y * mapSize.x).drawTerrain(renderWindow);
		}
		// Draw second-row
		for(unsigned int x = 0; x < mapSize.x; x += 2) {
			tileList.at(x + y * mapSize.x).drawTerrain(renderWindow);
		}
	}
	// Draw Roads
	for(unsigned int y = 0; y < mapSize.y; ++y) {
		// Draw top-row
		for(unsigned int x = 1; x < mapSize.x; x += 2) {
			tileList.at(x + y * mapSize.x).drawRoad(renderWindow);
		}
		// Draw second-row
		for(unsigned int x = 0; x < mapSize.x; x += 2) {
			tileList.at(x + y * mapSize.x).drawRoad(renderWindow);
		}
	}
}

const sf::Vector2u & Map::getMapSize() const {
	return mapSize;
}

bool Map::handleEvent(sf::Event event) {
	// Pass to child
	if(Widget::handleEvent(event)) {
		return true;
	}

    bool eventHandled = false;
    // Mouse button pressed
    if(event.type == sf::Event::MouseButtonPressed) {
        // If left mouse button pressed
        if(event.mouseButton.button == sf::Mouse::Button::Left) {
			if(!hasFocus()) {
				takeFocus();
			}
            // If clicked on ship
            //for(unsigned int factionIndex = 0; factionIndex < arena.getArenaFactionList()->size(); factionIndex++) {
            //    ArenaFaction* arenaFaction = arena.getArenaFactionList()->at(factionIndex);
            //    for(unsigned int shipIndex = 0; shipIndex < arenaFaction->getShipCount(); shipIndex++) {
            //        sf::Vector2f shipSpritePosition = arenaFaction->shipAt(shipIndex)->spritePosition_sf;
            //        sf::Vector2f shipSize = arenaFaction->shipAt(shipIndex)->getShipSize_sf();
            //        sf::Vector2f mousePosition = sf::Vector2f(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y);
            //        // If area of the triangles 
            //    }
            //}
        }
        // Middle mouse button pressed
        else if(event.mouseButton.button == sf::Mouse::Button::Middle) {
            isPanning = true;
            oldMousePoint = sf::Mouse::getPosition();
        }
    }
    // Mouse button released
    else if(event.type == sf::Event::MouseButtonReleased) {
        // Middle mouse button released
        if(event.mouseButton.button == sf::Mouse::Button::Middle) {
            isPanning = false;
        }
    }
    // Move mouse
    else if(event.type == sf::Event::MouseMoved) {
        // Pan camera
        if(isPanning == true) {
            view.move((oldMousePoint.x - event.mouseMove.x) * viewZoom / 4, (oldMousePoint.y - event.mouseMove.y) * viewZoom / 2);
            oldMousePoint = sf::Mouse::getPosition(*renderWindow_);

        }
    }
    // Mouse wheel moved
    else if(event.type == sf::Event::MouseWheelScrolled) {
        // Zoom)
        if(event.mouseWheelScroll.delta > 0) {
            zoomViewAt({event.mouseWheelScroll.x, event.mouseWheelScroll.y}, *renderWindow_, (1.f / 1.2f));
            viewZoom *= (1.f / 1.2f);
        } else if(event.mouseWheelScroll.delta < 0) {
            zoomViewAt({event.mouseWheelScroll.x, event.mouseWheelScroll.y}, *renderWindow_, 1.2f);
            viewZoom *= 1.2f;
        }
		std::wostringstream os_;
		os_ << "\n" << viewZoom;
		OutputDebugString(os_.str().c_str());
    }
    // Key pressed
    else if(event.type == sf::Event::KeyPressed) {
        if(event.key.code == sf::Keyboard::W) {
            panVerticalDirection = PanVerticalDirection::Up;
            panDelayCount = initPanDelay;
            view.move(sf::Vector2f(0, -panStep * viewZoom));
            eventHandled = true;
        } else if(event.key.code == sf::Keyboard::S) {
            panVerticalDirection = PanVerticalDirection::Down;
            panDelayCount = initPanDelay;
            view.move(sf::Vector2f(0, panStep * viewZoom));
            eventHandled = true;
        } else if(event.key.code == sf::Keyboard::A) {
            panHorizontalDirection = PanHorizontalDirection::Left;
            panDelayCount = initPanDelay;
            view.move(sf::Vector2f(-panStep * viewZoom, 0));
            eventHandled = true;
        } else if(event.key.code == sf::Keyboard::D) {
            panHorizontalDirection = PanHorizontalDirection::Right;
            view.move(sf::Vector2f(panStep * viewZoom, 0));
            panDelayCount = initPanDelay;
            eventHandled = true;
        }
    }
    // Key released
    else if(event.type == sf::Event::KeyReleased) {
        if(event.key.code == sf::Keyboard::W) {
            panVerticalDirection = PanVerticalDirection::VerticalNone;
            panDelayCount = initPanDelay;
            eventHandled = true;
        } else if(event.key.code == sf::Keyboard::S) {
            panVerticalDirection = PanVerticalDirection::VerticalNone;
            panDelayCount = initPanDelay;
            eventHandled = true;
        } else if(event.key.code == sf::Keyboard::A) {
            panHorizontalDirection = PanHorizontalDirection::HorizontalNone;
            panDelayCount = initPanDelay;
            eventHandled = true;
        } else if(event.key.code == sf::Keyboard::D) {
            panHorizontalDirection = PanHorizontalDirection::HorizontalNone;
            panDelayCount = initPanDelay;
            eventHandled = true;
        }
    }

    return eventHandled;
}

std::array<Tile*, 6> Map::getAdjacentTiles(const sf::Vector3i& pos) {
	return std::array<Tile*, 6> {
		getTilePtr(pos.x - 1, pos.y    , pos.z + 1),
		getTilePtr(pos.x - 1, pos.y + 1, pos.z    ),
		getTilePtr(pos.x    , pos.y + 1, pos.z - 1),
		getTilePtr(pos.x + 1, pos.y    , pos.z - 1),
		getTilePtr(pos.x + 1, pos.y - 1, pos.z    ),
		getTilePtr(pos.x    , pos.y - 1, pos.z + 1)
	};
}

const Tile& Map::getTile(const int & x, const int & y, const int & z) const {
	sf::Vector2i pos(Tile::getPos(x, y, z));
	return tileList.at(pos.y * mapSize.x + pos.x);
}

Tile* Map::getTile(const Tile& tile, const Map::Direction & direction) {
	switch(direction) {
	case Map::Direction::UP_LEFT:
		return getTilePtr(tile.getPos3().x - 1, tile.getPos3().y, tile.getPos3().z + 1);
	case Map::Direction::UP:
		return getTilePtr(tile.getPos3().x - 1, tile.getPos3().y + 1, tile.getPos3().z);
	case Map::Direction::UP_RIGHT:
		return getTilePtr(tile.getPos3().x, tile.getPos3().y + 1, tile.getPos3().z - 1);
	case Map::Direction::DOWN_RIGHT:
		return getTilePtr(tile.getPos3().x + 1, tile.getPos3().y, tile.getPos3().z - 1);
	case Map::Direction::DOWN:
		return getTilePtr(tile.getPos3().x + 1, tile.getPos3().y - 1, tile.getPos3().z);
	case Map::Direction::DOWN_LEFT:
		return getTilePtr(tile.getPos3().x, tile.getPos3().y - 1, tile.getPos3().z + 1);
	default:
		return nullptr;
	}
}

Tile& Map::getTile(const sf::Vector2f& pos) {
	/* A tile is two triangles and one square:
	      ____
		/|	  |\
		\|____|/
	*/

	// Determine Column
	/* A column is the right triangle(s) and square
		 |column|
	      ____  
		 |	  |\|
		 |____|/|
	*/
	unsigned int column = static_cast<unsigned int>(min(max((pos.x - 80) / 240, 0), mapSize.x - 1));
	// Determine row
	/* The row height depends on the column
                 ____
	      ____ /      \ ____	
		/ 	   \ 3__2 /      \
		\ 2__2 /      \ 4__2 /
		       \ ____ /
	*/
	unsigned int row = static_cast<unsigned int>(min(max((pos.y - (column % 2 ? 155 : 295)) / 280, 0), mapSize.y - 1));
	// If in triangle region
	/*      region
		      | |
	      ____  
		 |	  |\|
		 |____|/|
	*/
	if(static_cast<unsigned int>(pos.x - 80) % 240 > 160) {
		auto crossProduct = [](sf::Vector2f& a, sf::Vector2f& b)->bool {return a.x * b.y - a.y * b.x > 0; };
		/* Draw vectors of right triangle
			\ up vector
			/ down vector
		*/
		bool isRightofUpVector = crossProduct(sf::Vector2f(-80.f, -140.f), pos - getTile(column, row).getPixPos() - sf::Vector2f(320, 295));
		bool isRightofDownVector = crossProduct(sf::Vector2f(-80.f, 140.f), pos - getTile(column, row).getPixPos() - sf::Vector2f(320, 295));
		if(!isRightofUpVector && isRightofDownVector) {
			// Middle trangle
			return getTile(column, row);
		} else if(isRightofUpVector) {
			// Top triangle
			Tile* tile = getTile(getTile(column, row), Map::Direction::UP_RIGHT);
			if(tile != nullptr) {
				return *tile;
			} else {
				return getTile(column, row);
			}
		} else {
			// Bottom triangle
			Tile* tile = getTile(getTile(column, row), Map::Direction::DOWN_RIGHT);
			if(tile != nullptr) {
				return *tile;
			} else {
				return getTile(column, row);
			}
		}
	// Else not in triangle region
	} else {
		return getTile(column, row);
	}
}

//bool pathFound;

std::vector<Tile*> Map::getPath(Tile & start, Tile & finish) {
	struct Node {
		Node(Tile& tile, Node* back, Tile& finish) :
			back(back),
			tile(tile) {
			if(back != nullptr) {
				gCost = back->gCost + tile.getMovementCost();
				back->forward.push_back(this);
			} else {
				gCost = 0;
			}
			fCost = Tile::getDistance(tile, finish) + gCost;
		}
		~Node() {
			if(back != nullptr) {
				std::vector<Node*>::iterator backForwardNode = std::find_if(back->forward.begin(), back->forward.end(), [this](const Node* forwardIter)->bool {return this->tile.getPos2() == forwardIter->tile.getPos2(); });
				if(backForwardNode != back->forward.end()) {
					back->forward.erase(backForwardNode);
					//debugView.eraseLine(back->tile, tile);
				}
			}
			for(auto forwardIter : forward) {
				forwardIter->back = nullptr;
				//debugView.eraseLine(tile, forwardIter->tile);
			}
			//if(pathFound) {
			//	debugView.eraseCircle(tile);
			//	debugView.eraseText(tile);
			//	debugView.render();
			//}
		}
		bool operator==(const Node& other) const {
			if(tile.getPos2() == other.tile.getPos2()) {
				return true;
			} else {
				return false;
			}
		}
		sf::String getCost(Tile& finish) const {
			return sf::String(std::to_string(gCost) + std::string(": ") + (back != nullptr ? std::to_string(back->gCost) : std::string()) + std::string(" + ") + std::to_string(tile.getMovementCost()) + std::string("\n")
							  + std::to_string(fCost) + std::string(": ") + std::to_string(Tile::getDistance(tile, finish)));
		}
		sf::String getBackTile() const {
			return sf::String(std::to_string(back->tile.getPos2().x) + std::string(",") + std::to_string(back->tile.getPos2().y));
		}
		Tile& tile;
		Node* back;
		std::vector<Node*> forward;
		unsigned int fCost;	// Total Cost (gCost + estimated cost)
		unsigned int gCost;	// Cost of the path from start
	};
	//pathFound = false;
	std::list<Node> exploredPathList;
	std::list<Node> potentialPathList;
	potentialPathList.push_back(Node(start, nullptr, finish));
	// While we can still search
	while(!potentialPathList.empty()) {
		// find node with least cost
		potentialPathList.sort([](const Node& a, const Node& b) {return a.fCost < b.fCost; });
		exploredPathList.push_front(potentialPathList.front());
		potentialPathList.pop_front();
		// if least cost is end node
		if(exploredPathList.front().tile.getPos2() == finish.getPos2()) {
			// We're done
			break;
		}
		// Draw active node
		//debugView.addCircle(exploredPathList.front().tile, sf::Color(0x00, 0xFF, 0x00), 80.0f);
		//if(debugView.render()) {
		//	return std::vector<Tile*>();
		//}
		// Find adjacent tiles to least-cost node
		std::array<Tile*, 6> adjacentTiles = exploredPathList.front().tile.getAdjacentTiles();
		// For each adjacent tile
		for(std::array<Tile*, 6>::iterator tileIter = adjacentTiles.begin(); tileIter != adjacentTiles.end(); ++tileIter) {
			// If adjacent tile isn't in the map
			if(*tileIter == nullptr) {
				continue;
			}
			// Create a node for the adjacent tile
			Node adjacentNode(**tileIter, &exploredPathList.front(), finish);
			// Draw adjacent node
			//debugView.addCircle(adjacentNode.tile, sf::Color(sf::Color::Magenta), 80.0f);
			//debugView.addText(adjacentNode.tile, std::to_string(adjacentNode.gCost) + std::string("\n") + std::to_string(adjacentNode.fCost));
			//if(debugView.render()) {
			//	return std::vector<Tile*>();
			//}
			// Find matching node in explored list
			std::list<Node>::iterator exploredNode = std::find_if(exploredPathList.begin(), exploredPathList.end(), [adjacentNode](const Node& exploredNodeIter)->bool {return adjacentNode.tile.getPos2() == exploredNodeIter.tile.getPos2(); });
			// If this node is already in the explored list
			if(exploredNode != exploredPathList.end()) {
				//debugView.addCircle(adjacentNode.tile, sf::Color(0xFF, 0x7F, 0x7F), 80.0f);
				// Replace node if this node is cheaper
				if(adjacentNode.fCost < exploredNode->fCost) {
					//debugView.eraseLine(exploredNode->tile, exploredNode->back->tile);
					//debugView.addLine(adjacentNode.tile, adjacentNode.back->tile);
					//debugView.addCircle(adjacentNode.tile, sf::Color(0x7F, 0xFF, 0xFF), 80.0f);
					//debugView.addText(adjacentNode.tile, adjacentNode.getBackTile());
					potentialPathList.push_front(adjacentNode);
					for(std::vector<Node*>::iterator forwardIter = exploredNode->forward.begin(); forwardIter != exploredNode->forward.end(); ++forwardIter) {
						//debugView.eraseLine((*forwardIter)->tile, exploredNode->tile);
						(*forwardIter)->back = nullptr;
					}
					exploredPathList.erase(exploredNode);
				}
			// Else node isn't explored
			} else {
				// Find matching node in potential list
				std::list<Node>::iterator potentialNode = std::find_if(potentialPathList.begin(), potentialPathList.end(), [adjacentNode](const Node& exploredNodeIter)->bool {return adjacentNode.tile.getPos2() == exploredNodeIter.tile.getPos2(); });
				// If the adjacent node is not in the potential path list
				if(potentialNode == potentialPathList.end()) {
					potentialPathList.push_front(adjacentNode);
					//debugView.addLine(adjacentNode.tile, exploredPathList.front().tile);
					//debugView.addCircle(adjacentNode.tile, sf::Color(0x7F, 0x7F, 0xFF), 80.0f);
					//debugView.addText(adjacentNode.tile, adjacentNode.getBackTile());
				} else {
					// Replace node if this node is cheaper
					if(adjacentNode.fCost < potentialNode->fCost) {
						//debugView.eraseLine(potentialNode->tile, potentialNode->back->tile);
						//debugView.addLine(adjacentNode.tile, exploredPathList.front().tile);
						//debugView.addCircle(adjacentNode.tile, sf::Color(0x00, 0x00, 0x00), 80.0f);
						//debugView.addText(adjacentNode.tile, adjacentNode.getBackTile());
						potentialPathList.push_front(adjacentNode);
						for(std::vector<Node*>::iterator forwardIter = potentialNode->forward.begin(); forwardIter != potentialNode->forward.end(); ++forwardIter) {
							//debugView.eraseLine((*forwardIter)->tile, potentialNode->tile);
							(*forwardIter)->back = nullptr;
						}
						potentialPathList.erase(potentialNode);
					}
				}
			}
		}
		//debugView.addCircle(exploredPathList.front().tile, sf::Color(0xFF, 0x7F, 0x7F), 80.0f);
		//if(debugView.render()) {
		//	return std::vector<Tile*>();
		//}
	}
	//pathFound = true;
	if(exploredPathList.front().tile.getPos2() == finish.getPos2()) {
		// find path in reverse
		std::vector<Tile*> path;
		Node* node = &exploredPathList.front();
		while(node != nullptr) {
			path.push_back(&node->tile);
			//debugView.addCircle(node->tile, sf::Color::Green, 80.0f);
			node = node->back;
		}
		path.push_back(&start);
		//if(debugView.render()) {
		//	return std::vector<Tile*>();
		//}
		return path;
	} else {
		return std::vector<Tile*>();
	}
}

void Map::createRoad(Tile & tile) {
	tile.createRoad();
}

Tile& Map::getTile(const int & x, const int & y, const int & z) {
	sf::Vector2i pos(Tile::getPos(x, y, z));
	return getTile(pos.x, pos.y);
}

Tile & Map::getTile(const int & a, const int & b) {
	return tileList.at(b * mapSize.x + a);
}

Tile* Map::getTilePtr(const int & x, const int & y, const int & z) {
	sf::Vector2u pos(Tile::getPos(x, y, z));
	if((pos.x < 0) || (pos.y < 0) || (pos.x >= mapSize.x) || (pos.y >= mapSize.y)) {
		return nullptr;
	} else {
		return &tileList.at(pos.y * mapSize.x + pos.x);
	}
}

void Map::zoomViewAt(sf::Vector2i pixel, sf::RenderWindow& window, float zoom) {
    const sf::Vector2f beforeCoord{window.mapPixelToCoords(pixel, view)};
    view.zoom(zoom);
    window.setView(view);
    const sf::Vector2f afterCoord{window.mapPixelToCoords(pixel, view)};
    const sf::Vector2f offsetCoords{beforeCoord - afterCoord};
    view.move(offsetCoords);
    window.setView(view);
}
#pragma once
#include <SFML\Graphics.hpp>


class WorldPosition {
public:
	WorldPosition() {}
	~WorldPosition() {}
	sf::Vector2f getCoordinates(sf::Vector2i vector, sf::RenderWindow &window) {
		return window.mapPixelToCoords(vector);
	}
};
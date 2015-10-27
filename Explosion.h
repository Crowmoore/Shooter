#pragma once
#include <SFML\Graphics.hpp>

class Explosion : public sf::Sprite {
public:
	Explosion();
	~Explosion();

	void explode(sf::RenderWindow &, sf::Vector2f);

	int frameCount;
};
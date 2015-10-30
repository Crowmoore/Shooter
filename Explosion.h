#pragma once
#include <SFML\Graphics.hpp>

class Explosion : public sf::Sprite {
public:
	Explosion(sf::Texture &);
	~Explosion();

	void explode(sf::RenderWindow &);
	void draw(sf::RenderWindow &);

	int frameCount;
	bool hasExploded;
};
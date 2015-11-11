#pragma once
#include <SFML\Graphics.hpp>
#include "Player.h"

class Missile : public sf::Sprite {
public:
	Missile();
	~Missile();
	Missile(sf::Vector2f);
	float calculateRotation(sf::RenderWindow &, Player &);
	void draw(sf::RenderWindow &);

	int damage;
	int score;
	sf::Vector2f velocity;

private:
	sf::Texture tex;
};

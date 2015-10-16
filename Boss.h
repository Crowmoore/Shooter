#pragma once
#include <SFML\Graphics.hpp>
#include <string>
#include "Player.h"

using namespace std;

class Boss : public sf::CircleShape {
public:
	Boss();
	~Boss();
	Boss(string, sf::Texture *, sf::Vector2f);
	void draw(sf::RenderWindow &);
	float getHealth();
	float calculateRotation(sf::RenderWindow &, Player &, sf::Vector2f);

	int score;
	bool encounterHasStarted;
	float health;
	float damage;
	sf::RectangleShape hardpoint1;
	sf::RectangleShape hardpoint2;
	sf::Vector2f velocity;
};

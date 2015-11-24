#pragma once
#include "Enemy.h"

class Drone : public Enemy {
public:
	Drone();
	~Drone();
	Drone(sf::Vector2f);

	float calculateRotation(sf::RenderWindow &, Player &);
	void shoot(sf::RenderWindow &, Player &, vector <Enemy *> &, vector <Bullet *> &, vector <Missile *> &);
	void animate();
private:
	sf::Texture tex;
};
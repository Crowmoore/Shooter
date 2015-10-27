#pragma once
#include "Enemy.h"

class Drone : public Enemy {
public:
	Drone();
	~Drone();
	Drone(sf::Vector2f);

	float calculateRotation(sf::RenderWindow &, Player &);
	void update(sf::RenderWindow &, Player &, vector<Enemy *> &, vector<Bullet> &);
	void shoot(sf::RenderWindow &, Player &, vector <Enemy *> &, vector <Bullet> &);
	void animate();

	sf::Texture tex;
};
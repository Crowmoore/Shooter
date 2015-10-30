#pragma once
#include "Enemy.h"

class Marauder : public Enemy {
public:
	Marauder();
	~Marauder();
	Marauder(sf::Vector2f);
	float calculateRotation(sf::RenderWindow &, Player &);
	void update(sf::RenderWindow &, Player &, vector<Enemy *> &, vector<Bullet *> &);
	void shoot(sf::RenderWindow &, Player &, vector <Enemy *> &, vector <Bullet *> &);
	void animate();

	sf::Texture tex;
	int frameCount;
};
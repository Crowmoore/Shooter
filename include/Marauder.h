#pragma once
#include "Enemy.h"

class Marauder : public Enemy {
public:
	Marauder();
	~Marauder();
	Marauder(sf::Vector2f);
	float calculateRotation(sf::RenderWindow &, Player &);
	void shoot(sf::RenderWindow &, Player &, vector <Enemy *> &, vector <Bullet *> &, vector <Missile *> &);
	void animate();

	
private:
	int frameCount;
	sf::Texture tex;
};
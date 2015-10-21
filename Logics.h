#pragma once

#include "Enemy.h"
#include "Player.h"
#include "Bullet.h"
#include "SFML\Audio.hpp"
#include <vector>


using namespace std;

class Logics {
public:
	Logics();
	~Logics();
	
	void destroyOutOfBoundsBullets(vector <Bullet> &, float, float, float, float);
	void resolveBulletHitsOnPlayer(sf::RenderWindow &, vector <Bullet> &, Player &);
	void resolveBulletHitsOnEnemy(vector <Bullet> &, vector <Enemy> &, Player &, sf::Sound &);
	//void resolveCollision(vector <Enemy> &, Player &);
};
#pragma once

#include "Enemy.h"
#include "Player.h"
#include "Bullet.h"
#include "SFML\Audio.hpp"
#include <vector>
#include "Powerups.h"
#include "Explosion.h"

using namespace std;

class Logics {
public:
	Logics();
	~Logics();
	
	void destroyOutOfBoundsBullets(vector <Bullet *> &, sf::FloatRect);
	void resolveBulletHitsOnPlayer(sf::RenderWindow &, vector <Bullet *> &, Player &);
	void resolveBulletHitsOnEnemy(sf::RenderWindow &, vector <Bullet *> &, vector <Enemy *> &, Player &, sf::Sound &, vector <Powerups *> &, vector <Explosion *> &, sf::Texture *);
	void updateExplosions(sf::RenderWindow &, vector <Explosion *> &);
	void resolveCollisions(vector <Enemy *> &, Player &);
	void updateEnemies(sf::RenderWindow &, Player &, vector<Enemy *> &, vector<Bullet *> &);
	void updatePowerups(sf::RenderWindow &, vector <Powerups *> &, Player &, sf::Sound &);
	Powerups* spawnRandomPowerUp();
	

};
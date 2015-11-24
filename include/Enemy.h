#pragma once
#include <SFML\Graphics.hpp>
#include <iostream>
#include <string>
#include "Player.h"
#include "Bullet.h"
#include "Loader.h"
#include "Missile.h"

using namespace std;

class Enemy : public sf::Sprite {
public:
	Enemy();
	~Enemy();
	virtual float calculateRotation(sf::RenderWindow &, Player &) = 0;
	virtual void shoot(sf::RenderWindow &, Player &, vector <Enemy *> &, vector <Bullet *> &, vector <Missile *> &) = 0;
	virtual void animate() = 0;
	void draw(sf::RenderWindow &window);
	int getScore();
	void setScore(int);
	float getHealth();
	void setHealth(float);
	sf::Vector2f getVelocity();
	void setVelocity(sf::Vector2f);
protected:
	int score;
	float health;
	float damage;
	sf::Vector2f velocity;
};
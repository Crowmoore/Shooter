#pragma once
#include <SFML\Graphics.hpp>
#include <iostream>
#include <string>
#include "Player.h"
#include "Bullet.h"

using namespace std;

class Enemy : public sf::RectangleShape {
public:
	Enemy();
	~Enemy();
	Enemy(string, sf::Texture *, sf::Vector2f);
	void draw(sf::RenderWindow &);
	float calculateRotation(sf::RenderWindow &, Player &);
	void resolveCollisions(vector <Enemy> &, Player &);
	void updateEnemies(sf::RenderWindow &, Player &, vector<Enemy> &, vector<Bullet> &);

	int score;
	float health;
	float damage;
	sf::Vector2f velocity;
};
#pragma once
#include <string>
#include <SFML\Graphics.hpp>
#include <iostream>
#include <math.h>

using namespace std;

class Player : public sf::RectangleShape {
public:
	Player();
	~Player();
	Player(string, sf::Texture *);

	void draw(sf::RenderWindow &);
	void lookAtCursor(sf::RenderWindow &, sf::View &);
	void adjustVelocity();
	void setAmmoDescription(string);
	void checkBounds(Player &, float, float, float, float);
	float getShieldCharge();
	void setShieldCharge(float);
	string getAmmoDescription();

	int health;
	int score;
	float shieldCharge;
	string ammoDescription;
	float damage;
	bool isShielded;
	bool alive;
	float acceleration;
	sf::Vector2f velocity;
	float maxVelocity;
	sf::Vector2f spawnPoint;
};
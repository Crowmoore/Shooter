#pragma once
#include <string>
#include "Bullet.h"
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <iostream>
#include <math.h>
#include <vector>


using namespace std;

class Player : public sf::RectangleShape {
public:
	Player();
	~Player();
	Player(string, sf::Texture *, sf::Texture *);

	void draw(sf::RenderWindow &);
	void lookAtCursor(sf::RenderWindow &, sf::View &);
	void adjustVelocity();
	void setAmmoDescription(string);
	void checkBounds(Player &, float, float, float, float);
	void updatePlayer(Player &, float, float, float, float);
	void shoot(sf::RenderWindow &, vector <Bullet> &, sf::Sound &);
	void activateShield(sf::RenderWindow &);
	void checkHealth(sf::Sound &);
	float getShieldCharge();
	void setShieldCharge(float);
	void drawShieldMeter(sf::RenderWindow &);
	string getAmmoDescription();

	sf::RectangleShape shieldMeterBlue;
	sf::RectangleShape shieldMeterBlack;
	int health;
	int score;
	float shieldCharge;
	sf::Sound heartbeat;
	string ammoDescription;
	float damage;
	bool isShielded;
	bool alive;
	bool isDying;
	sf::Sprite shield;
	float acceleration;
	sf::Vector2f velocity;
	float maxVelocity;
	sf::Vector2f spawnPoint;
};
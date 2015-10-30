#pragma once
#include <string>
#include "Bullet.h"
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <iostream>
#include <math.h>
#include <vector>

using namespace std;

class Player : public sf::Sprite {
public:
	Player();
	~Player();

	void draw(sf::RenderWindow &);
	void lookAtCursor(sf::RenderWindow &, sf::View &);
	void adjustVelocity();
	void setAmmoDescription(string);
	void checkBounds(Player &, sf::FloatRect);
	void update(sf::RenderWindow &, vector <Bullet *> &, sf::Sound &, sf::FloatRect, sf::Clock &);
	void shoot(sf::RenderWindow &, vector <Bullet *> &, sf::Sound &);
	void activateShield(sf::RenderWindow &);
	void checkHealth(sf::Sound &);
	float getShieldCharge();
	void setShieldCharge(float);
	void animate();
	void drawShieldMeter(sf::RenderWindow &);
	string getAmmoDescription();

	sf::RectangleShape shieldMeterBlue;
	sf::RectangleShape shieldMeterBlack;
	int health;
	int score;
	sf::Texture shieldTex;
	sf::Texture tex;
	float rateOfFire;
	float shieldCharge;
	sf::Sound heartbeat;
	string ammoDescription;
	float damage;
	int frameCount;
	bool isShielded;
	bool alive;
	int pointMultiplier;
	bool isDying;
	sf::Sprite shield;
	float acceleration;
	sf::Vector2f velocity;
	float maxVelocity;
	sf::Vector2f spawnPoint;
};
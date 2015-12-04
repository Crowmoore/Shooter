#pragma once
#include <string>
#include "Bullet.h"
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <iostream>
#include <math.h>
#include <vector>
#include "Loader.h"

using namespace std;

class Player : public sf::Sprite {
public:
	Player();
	~Player();

	void draw(sf::RenderWindow &);
	void lookAtCursor(sf::RenderWindow &, sf::View &);
	void adjustVelocity();
	void checkBounds(Player &, sf::FloatRect);
	void update(sf::RenderWindow &, vector <Bullet *> &, sf::Sound &, sf::FloatRect, sf::Clock &);
	void shoot(sf::RenderWindow &, vector <Bullet *> &, sf::Sound &);
	void activateShield(sf::RenderWindow &);
	void checkHealth(sf::Sound &);
	void animate();
	string getAmmoDescription();
	void setAmmoDescription(string);
	int getHealth();
	void setHealth(int);
	float getAcceleration();
	void setAcceleration(float);
	float getRateOfFire();
	void setRateOfFire(float);
	float getShieldCharge();
	void setShieldCharge(float);
	float getDamage();
	void setDamage(float);
	bool getShielded();
	void setShielded(bool);
	bool getAlive();
	void setAlive(bool);
	sf::Vector2f getVelocity();
	void setVelocity(sf::Vector2f);
	sf::Vector2f getSpawnPoint();
	void setSpawnPoint(sf::Vector2f);
	int getPointMultiplier();
	void setPointMultiplier(int);

	sf::Sprite shield;
	
private:
	int health;
	int pointMultiplier;
	bool isDying;
	bool alive;
	bool isShielded;
	int frameCount;
	float maxVelocity;
	float damage;
	float rateOfFire;
	float shieldCharge;
	float acceleration;
	string ammoDescription;
	sf::Vector2f spawnPoint;
	sf::Vector2f velocity;
	sf::Texture shieldTex;
	sf::Texture tex;
	sf::Sound heartbeat;
	sf::FloatRect bounds;
};
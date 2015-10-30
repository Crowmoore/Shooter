#pragma once
#include <SFML\Graphics.hpp>
#include <string>
#include <iostream>

using namespace std;

class Bullet : public sf::Sprite {
public:
	Bullet();
	~Bullet();
	Bullet(sf::Vector2f, sf::Vector2f, sf::Color, string);

	void calculateDirection(sf::RenderWindow &);
	float calculateRotation(sf::RenderWindow &);
	void draw(sf::RenderWindow &);
	float damage;
	float rateOfFire;
	string id;

	sf::Texture tex;
	sf::Vector2f velocity;
	sf::Vector2f position;
	sf::Vector2f target;

};
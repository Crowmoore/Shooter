#pragma once
#include <SFML\Graphics.hpp>
#include <string>
#include <iostream>
#include "Player.h"

using namespace std;

class Ammunition : public sf::RectangleShape {
public:
	Ammunition();
	~Ammunition();
	Ammunition(sf::Vector2f, sf::Vector2f, sf::Color, string);

	void calculateDirection(sf::RenderWindow &);
	void calculateRotation(sf::RenderWindow &);
	void draw(sf::RenderWindow &);
	float damage;
	float rateOfFire;
	string id;

	//sf::Sprite bulletSprite;
	sf::Vector2f velocity;
	sf::Vector2f position;
	sf::Vector2f target;

};
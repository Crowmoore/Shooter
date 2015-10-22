#pragma once
#include <SFML\Graphics.hpp>
#include <string>

using namespace std;

class Multiplier : public sf::CircleShape {
public:
	Multiplier();
	~Multiplier();
	Multiplier(string, sf::Vector2f);

	void draw(sf::RenderWindow &);
	int value;
	sf::Texture tex;
};
#pragma once
#include <SFML\Graphics.hpp>
#include <vector>
#include "Loader.h"

using namespace std;

class Powerups : public sf::CircleShape {
public:
	Powerups();
	~Powerups();
	Powerups(string, string);
	void draw(sf::RenderWindow &);
	string getId();
	void setId(string);
private:
	string id;
	sf::CircleShape circle;
	sf::Texture tex;
};
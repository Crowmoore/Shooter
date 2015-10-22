#pragma once
#include <SFML\Graphics.hpp>
#include <vector>

using namespace std;

class Powerups : public sf::CircleShape {
public:
	Powerups();
	~Powerups();
	Powerups(string, string);
	void draw(sf::RenderWindow &);

	string id;
	sf::CircleShape circle;
	sf::Texture tex;
};
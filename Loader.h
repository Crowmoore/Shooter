#pragma once
#include <string>
#include <iostream>
#include <SFML\Graphics.hpp>

using namespace std;

class Loader {
public:
	Loader();
	~Loader();

	sf::Texture loadTexture(string);
	//sf::Sprite loadSprite(string);
	sf::Font loadFont(string);
};

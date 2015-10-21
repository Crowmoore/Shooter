#pragma once
#include <string>
#include <iostream>
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>

using namespace std;

class Loader {
public:
	Loader();
	~Loader();

	sf::Texture loadTexture(string);
	//sf::Sprite loadSprite(string);
	void loadMusic(string);
	sf::Font loadFont(string);
	sf::Music music;
};

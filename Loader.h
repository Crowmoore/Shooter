#pragma once
#include <string>
#include <iostream>
#include <SFML\Graphics.hpp>
#include <fstream>

using namespace std;

class Loader {
public:
	Loader();
	~Loader();

	sf::Texture loadTexture(string);
	//sf::Sprite loadSprite(string);
	sf::Font loadFont(string);
	void saveHighscoreToFile(int score);
	int loadHighscoreFromFile();
};

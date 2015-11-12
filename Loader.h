#pragma once
#include <string>
#include <iostream>
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <fstream>

using namespace std;

class Loader {
public:
	Loader();
	~Loader();
	sf::Texture loadTexture(string);
	sf::Font loadFont(string);
	sf::Image loadImage(string);
	sf::Sound loadSound(string);
	void saveHighscoreToFile(int score);
	void loadMusic(string);
	void createSaveFile();
	int loadHighscoreFromFile();
};

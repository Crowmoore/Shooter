#pragma once
#include <string>
#include <iostream>
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <fstream>
#include <vector>


using namespace std;

class Loader {
public:
	Loader();
	~Loader();
	sf::Texture loadTexture(string);
	sf::Font loadFont(string);
	sf::Image loadImage(string);
	sf::Sound loadSound(string);
	void saveHighscoreToFile(vector <int>);
	void loadMusic(string);
	void createSaveFile();
	vector <int> loadHighscoreFromFile();
};

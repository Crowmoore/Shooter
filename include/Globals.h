#pragma once
#include <SFML\Audio.hpp>
#include <vector>

int musicVolume = 80;
int enemiesKilled = 0;
int points = 0;
int survivalBonus = 0;
int currentLevel = 0;
int level1Highscore = 0;
int level2Highscore = 0;
std::vector <int> highscores(2,0);
sf::Music music;

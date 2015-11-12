#ifndef globalsh
#define globalsh
#include <SFML\Audio.hpp>

int musicVolume = 80;
int enemiesKilled = 0;
int points = 0;
int survivalBonus = 0;
int level1Highscore = 0;
struct Score {
	int level1;
	int level2;
} highscore;
sf::Music music;

#endif
#pragma once
#include <SFML\Graphics.hpp>
#include <vector>
#include "Globals.h"

using namespace std;

class Screen {
public:
	virtual int run(sf::RenderWindow &window) = 0;
	bool isRunning;
};
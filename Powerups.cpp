#include "include/Powerups.h"

Powerups::Powerups() {}
Powerups::~Powerups() {}

//Constructor takes in the texture path and an id for the Power-Up.
Powerups::Powerups(string path, string id) {	
	Loader loader;
	this->setRadius(20);
	this->tex = loader.loadTexture(path);
	this->tex.setSmooth(true);
	this->setTexture(&tex);
	this->id = id;
}
//Draw the Power-Up.
void Powerups::draw(sf::RenderWindow &window) {
	window.draw(*this);
}

string Powerups::getId()
{
	return this->id;
}

void Powerups::setId(string id)
{
	this->id = id;
}

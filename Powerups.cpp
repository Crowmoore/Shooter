#include "Powerups.h"

Powerups::Powerups() {}
Powerups::~Powerups() {}

Powerups::Powerups(string path, string id) {	
	Loader loader;
	this->setRadius(20);
	this->tex = loader.loadTexture(path);
	this->tex.setSmooth(true);
	this->setTexture(&tex);
	this->id = id;
}

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

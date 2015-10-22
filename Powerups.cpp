#include "Powerups.h"

Powerups::Powerups() {}
Powerups::~Powerups() {}

Powerups::Powerups(string path, string id) {	
	this->setRadius(20);
	this->tex.loadFromFile(path);
	this->tex.setSmooth(true);
	this->setTexture(&tex);
	this->id = id;
}

void Powerups::draw(sf::RenderWindow &window) {
	window.draw(*this);
}

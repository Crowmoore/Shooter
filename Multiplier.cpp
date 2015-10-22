#include "Multiplier.h"

Multiplier::Multiplier() {}
Multiplier::~Multiplier() {}
Multiplier::Multiplier(string path, sf::Vector2f position) {
	this->tex.loadFromFile(path);
	this->tex.setSmooth(true);
	this->setTexture(&tex);
	this->setPosition(position);
	this->setRadius(20);
}
void Multiplier::draw(sf::RenderWindow &window) {
	window.draw(*this);
}
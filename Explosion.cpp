#include "Explosion.h"
#include <iostream>

using namespace std;
//Contructor only takes in the reference to the spritesheet texture.
Explosion::Explosion(sf::Texture &tex) {
	this->frameCount = 0;
	this->setTexture(tex);
	this->setTextureRect(sf::IntRect(0, 0, 256, 256));
	this->setOrigin(this->getLocalBounds().width / 2, this->getLocalBounds().height / 2);
	this->hasExploded = false;
}
Explosion::~Explosion() {}

bool Explosion::getExploded()
{
	return this->hasExploded;
}

void Explosion::setExploded(bool exploded)
{
	this->hasExploded = exploded;
}
//Loop through the spritesheet and set this to exploded when finished.
void Explosion::explode(sf::RenderWindow &window) {
	this->setTextureRect(sf::IntRect(256 * this->frameCount, 0, 256, 256));
	this->frameCount++;
	window.draw(*this);
	if (this->frameCount >= 48) {
		this->hasExploded = true;
	}
}
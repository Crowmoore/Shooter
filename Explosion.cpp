#include "Explosion.h"

Explosion::Explosion() {
	this->frameCount = 0;
	sf::Texture texture;
	texture.loadFromFile("assets/pics/explosion.png");
	this->setTexture(texture);
}
Explosion::~Explosion() {}

void Explosion::explode(sf::RenderWindow &window, sf::Vector2f position) {
	this->setPosition(position);
	for (this->frameCount = 0; this->frameCount < 48; this->frameCount++) {
		this->setTextureRect(sf::IntRect(256 * this->frameCount, 0, 256, 256));
		window.draw(*this);
	}
}
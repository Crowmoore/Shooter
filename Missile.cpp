#include "Missile.h"

Missile::Missile() {}
Missile::~Missile() {}
//Constructor. Only takes in the spawn position and initializes the Missile.
Missile::Missile(sf::Vector2f spawn) {
	Loader loader;
	this->tex = loader.loadTexture("assets/pics/missile.png");
	this->setTexture(tex);
	this->setScale(0.2, 0.2);

	this->setOrigin(this->getLocalBounds().width / 2, this->getLocalBounds().height / 2);
	this->setPosition(spawn);
	this->damage = 20;
	this->score = 5;
	this->velocity = sf::Vector2f(5, 5);
}
//Draw the missile.
void Missile::draw(sf::RenderWindow &window) {
	window.draw(*this);
}
//Calculate and set the rotation so the Missile always looks at the player.
float Missile::calculateRotation(sf::RenderWindow &window, Player &player) {
	sf::Vector2f currentPosition = this->getPosition();
	sf::Vector2f playerPosition = player.getPosition();

	const float pi = 3.14159265;

	float dx = playerPosition.x - currentPosition.x;
	float dy = playerPosition.y - currentPosition.y;

	float degrees = (atan2(dy, dx)) * 180.0 / pi;
	float radians = (atan2(dy, dx));
	this->setRotation(degrees + 90);
	return radians;
}


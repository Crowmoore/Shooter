#include "Marauder.h"

Marauder::Marauder() {}
Marauder::~Marauder() {}
//Constructor. Only takes in the spawn position and initializes the Marauder.
Marauder::Marauder(sf::Vector2f spawn) {
	Loader loader;
	this->tex = loader.loadTexture("assets/pics/marauder.png");
	this->setTexture(tex);
	
	this->setTextureRect(sf::IntRect(0, 0, 162, 234));
	this->setOrigin(this->getLocalBounds().width / 2, this->getLocalBounds().height / 2);
	this->setScale(sf::Vector2f(0.7, 0.7));
	this->setPosition(spawn);
	this->setRotation(0);
	this->damage = 20;
	this->score = 40;
	this->health = 120;
	this->velocity = sf::Vector2f(1.5, 1.5);
	this->frameCount = 0;
}
//Calculate and set the rotation so the Marauder always looks at the player.
float Marauder::calculateRotation(sf::RenderWindow &window, Player &player) {
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
//Loop through the spritesheet.
void Marauder::animate() {
	this->setTextureRect(sf::IntRect(162 * frameCount, 0, 162, 234));
	frameCount++;
	if (frameCount > 2) {
		frameCount = 0;
	}
}
//Check if the Marauder should shoot. Create and initialize a missile and push it to missiles vector.
void Marauder::shoot(sf::RenderWindow &window, Player &player, vector <Enemy *> &enemies, vector <Bullet *> &bullets, vector <Missile *> &missiles) {

	int random = rand() % 500 + 1;
	if (random == 1) {
		Missile* missile = new Missile(this->getPosition());
		missile->velocity = (sf::Vector2f(5, 5));
		missile->damage = (this->damage);
		missile->calculateRotation(window, player);
		missiles.push_back(missile);
	}

}
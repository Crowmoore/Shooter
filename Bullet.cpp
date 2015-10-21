#include "Bullet.h"



Bullet::Bullet() {}
Bullet::~Bullet() {}
Bullet::Bullet(sf::Vector2f spawnPosition, sf::Vector2f targetPosition, sf::Color color, string id) {
	/*sf::Image image;
	sf::Texture tex;
	if (!image.loadFromFile(pathName)) {
		cout << "Could not open image: " << pathName << endl;
	}
	else {
		image.loadFromFile(pathName);
		tex.loadFromImage(image);
	}*/
	this->setSize(sf::Vector2f(3, 10));
	this->setFillColor(color);
	this->id = id;
	this->position = spawnPosition;
	this->target = targetPosition;
	this->setPosition(position);
	this->velocity = sf::Vector2f(5.f ,5.f);
	this->damage = 0.f;
}
void Bullet::draw(sf::RenderWindow &window) {
	window.draw(*this);
}
void Bullet::calculateRotation(sf::RenderWindow &window) {
	sf::Vector2f currentPosition = this->getPosition();
	sf::Vector2f targetPosition = this->target;
	//cursorPosition = sf::Vector2i(window.mapPixelToCoords(cursorPosition, view));
	const float pi = 3.14159265;

	float dx = targetPosition.x - currentPosition.x;
	float dy = targetPosition.y - currentPosition.y;

	float rotation = (atan2(dy, dx)) * 180.0 / pi;
	
	this->setRotation(rotation + 90);
}
void Bullet::calculateDirection(sf::RenderWindow &window) {
	sf::Vector2f currentPosition = this->getPosition();
	sf::Vector2f targetPosition = target;

	float distance = sqrt(((targetPosition.x - currentPosition.x) * (targetPosition.x - currentPosition.x) + (targetPosition.y - currentPosition.y) * (targetPosition.y - currentPosition.y)));
	this->velocity.x = this->velocity.x * (targetPosition.x - currentPosition.x) / distance;
	this->velocity.y = this->velocity.y * (targetPosition.y - currentPosition.y) / distance;
}
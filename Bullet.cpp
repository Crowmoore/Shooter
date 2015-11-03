#include "Bullet.h"



Bullet::Bullet() {}
Bullet::~Bullet() {}
Bullet::Bullet(sf::Vector2f spawnPosition, sf::Vector2f targetPosition, sf::Color color, string id) {
	if (!this->tex.loadFromFile("assets/pics/bullet.png")) {
		cout << "Could not open image: assets/pics/bullet.png" << endl;
	}
	this->tex.loadFromFile("assets/pics/bullet.png");
	this->setTexture(tex);
	this->setScale(sf::Vector2f(0.5, 0.5));
	this->setColor(color);
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
float Bullet::getDamage()
{
	return this->damage;
}
void Bullet::setDamage(float damage)
{
	this->damage = damage;
}
string Bullet::getId()
{
	return this->id;
}
void Bullet::setId(string id)
{
	this->id = id;
}
sf::Vector2f Bullet::getVelocity()
{
	return this->velocity;
}
void Bullet::setVelocity(sf::Vector2f velocity)
{
	this->velocity = velocity;
}
float Bullet::calculateRotation(sf::RenderWindow &window) {
	sf::Vector2f currentPosition = this->getPosition();
	sf::Vector2f targetPosition = this->target;
	const float pi = 3.14159265;

	float dx = targetPosition.x - currentPosition.x;
	float dy = targetPosition.y - currentPosition.y;

	float degrees = (atan2(dy, dx)) * 180.0 / pi;
	float radians = (atan2(dy, dx));
	this->setRotation(degrees + 90);
	return radians;
}
void Bullet::calculateDirection(sf::RenderWindow &window) {
	sf::Vector2f currentPosition = this->getPosition();
	sf::Vector2f targetPosition = this->target;

	float distance = sqrt(((targetPosition.x - currentPosition.x) * (targetPosition.x - currentPosition.x) + (targetPosition.y - currentPosition.y) * (targetPosition.y - currentPosition.y)));
	this->velocity.x = this->velocity.x * (targetPosition.x - currentPosition.x) / distance;
	this->velocity.y = this->velocity.y * (targetPosition.y - currentPosition.y) / distance;
}
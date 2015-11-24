#include "include/Enemy.h"

//Base class for Drones and Marauders
Enemy::Enemy() {}
Enemy::~Enemy() {}

void Enemy::draw(sf::RenderWindow &window) {
	window.draw(*this);
}

int Enemy::getScore()
{
	return this->score;
}

void Enemy::setScore(int score)
{
	this->score = score;
}

float Enemy::getHealth()
{
	return this->health;
}

void Enemy::setHealth(float health)
{
	this->health = health;
}

sf::Vector2f Enemy::getVelocity()
{
	return this->velocity;
}

void Enemy::setVelocity(sf::Vector2f velocity)
{
	this->velocity = velocity;
}

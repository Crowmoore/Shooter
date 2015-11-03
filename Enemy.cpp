#include "Enemy.h"

Enemy::Enemy() {}
Enemy::~Enemy() {}
Enemy::Enemy(string pathName, sf::Texture *tex, sf::Vector2f spawn) {}

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

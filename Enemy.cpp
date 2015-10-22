#include "Enemy.h"

Enemy::Enemy() {}
Enemy::~Enemy() {}
Enemy::Enemy(string pathName, sf::Texture *tex, sf::Vector2f spawn) {}

void Enemy::draw(sf::RenderWindow &window) {
	window.draw(*this);
}

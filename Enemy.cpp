#include "Enemy.h"

Enemy::Enemy() {}
Enemy::~Enemy() {}
Enemy::Enemy(string pathName, sf::Texture *tex, sf::Vector2f spawn) {
	sf::Image image;
	if (!image.loadFromFile(pathName)) {
		cout << "Could not open image: " << pathName << endl;
	}
	else {
		image.loadFromFile(pathName);
		image.createMaskFromColor(sf::Color::White);
		tex->loadFromImage(image);
	}
	this->setSize(sf::Vector2f(64, 64));
	this->setOrigin(32, 32);
	this->setPosition(spawn);
	this->setTexture(tex);
	this->setRotation(0);
	this->damage = 5;
	this->score = 10;
	this->health = 100;
	this->velocity = sf::Vector2f(2.f, 2.f);
}
void Enemy::draw(sf::RenderWindow &window) {
	window.draw(*this);
}
float Enemy::calculateRotation(sf::RenderWindow &window, Player &player) {
	sf::Vector2f currentPosition = this->getPosition();
	sf::Vector2f playerPosition = player.getPosition();
	//cursorPosition = sf::Vector2i(window.mapPixelToCoords(cursorPosition, view));
	const float pi = 3.14159265;

	float dx = playerPosition.x - currentPosition.x;
	float dy = playerPosition.y - currentPosition.y;

	float degrees = (atan2(dy, dx)) * 180.0 / pi;
	float radians = (atan2(dy, dx));
	this->setRotation(degrees + 90);
	return radians;
}
/*float Enemy::calculateDistance(sf::RenderWindow &window, Player &player) {
	sf::Vector2f currentPosition = this->getPosition();
	sf::Vector2f playerPosition = player.getPosition();

	float distance = sqrt(((playerPosition.x - currentPosition.x) * (playerPosition.x - currentPosition.x) + (playerPosition.y - currentPosition.y) * (playerPosition.y - currentPosition.y)));
	this->velocity.x = this->velocity.x * (playerPosition.x - currentPosition.x) / distance;
	this->velocity.y = this->velocity.y * (playerPosition.y - currentPosition.y) / distance;
	return distance;
}*/

void Enemy::resolveCollisions(vector <Enemy> &enemies, Player &player) {
	for (int i = 0; i < enemies.size(); i++) {
		if (player.getGlobalBounds().intersects(enemies[i].getGlobalBounds())) {
			enemies[i].move((enemies[i].getSize().x / 8) - (player.getOrigin().x / 64), (enemies[i].getSize().y / 8) - (player.getOrigin().y / 64));
		}
		for (int j = i + 1; j < enemies.size(); j++) {
			if (enemies[i].getGlobalBounds().intersects(enemies[j].getGlobalBounds())) {
				enemies[i].move((enemies[i].getSize().x / 16) - (enemies[j].getOrigin().x / 32), (enemies[i].getSize().y / 16) - (enemies[j].getOrigin().y / 32));
			}

		}
	}
}
void Enemy::updateEnemies(sf::RenderWindow &window, Player &player, vector<Enemy> &enemies, vector<Ammunition> &bullets) {
	for (int i = 0; i < enemies.size(); i++) {
		enemies[i].calculateRotation(window, player);
		enemies[i].setPosition(enemies[i].getPosition() + sf::Vector2f(cos(enemies[i].calculateRotation(window, player)) * enemies[i].velocity.x, sin(enemies[i].calculateRotation(window, player)) * enemies[i].velocity.y));
		enemies[i].resolveCollisions(enemies, player);
		int random = rand() % 50 + 1;
		enemies[i].draw(window);
		if (random == 1) {
			Ammunition ammo(enemies[i].getPosition(), player.getPosition(), sf::Color::Green, "enemy");
			ammo.velocity = sf::Vector2f(8, 8);
			ammo.setSize(sf::Vector2f(6, 13));
			ammo.damage = enemies[i].damage;
			ammo.calculateRotation(window);
			ammo.calculateDirection(window);
			bullets.push_back(ammo);
		}
	}
}
#include "Marauder.h"

Marauder::Marauder() {}
Marauder::~Marauder() {}
Marauder::Marauder(sf::Vector2f spawn) {
	sf::Image image;
	if (!image.loadFromFile("assets/pics/link.jpg")) {
		cout << "Could not open image: assets/pics/link.jpg" << endl;
	}
	image.loadFromFile("assets/pics/link.jpg");
	image.createMaskFromColor(sf::Color::White);
	this->tex.loadFromImage(image);

	this->setSize(sf::Vector2f(128, 128));
	this->setOrigin(64, 64);
	this->setPosition(spawn);
	this->setTexture(&tex);
	this->setRotation(0);
	this->damage = 20;
	this->score = 40;
	this->health = 120;
	this->velocity = sf::Vector2f(1.f, 1.f);
	this->hasRockets = true;
}

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

void Marauder::update(sf::RenderWindow &window, Player &player, vector<Enemy *> &enemies, vector<Bullet> &bullets) {
	for (int i = 0; i < enemies.size(); i++) {
		enemies[i]->calculateRotation(window, player);
		enemies[i]->setPosition(enemies[i]->getPosition() + sf::Vector2f(cos(enemies[i]->calculateRotation(window, player)) * enemies[i]->velocity.x, sin(enemies[i]->calculateRotation(window, player)) * enemies[i]->velocity.y));
		enemies[i]->shoot(window, player, enemies, bullets);
		enemies[i]->draw(window);
	}
}
void Marauder::shoot(sf::RenderWindow &window, Player &player, vector <Enemy *> &enemies, vector <Bullet> &bullets) {

		int random = rand() % 300 + 1;
		if (random == 1) {
			Bullet ammo(this->getPosition(), player.getPosition(), sf::Color::Blue, "enemy");
			ammo.velocity = sf::Vector2f(4, 4);
			ammo.setSize(sf::Vector2f(10, 18));
			ammo.damage = this->damage;
			ammo.calculateRotation(window);
			ammo.calculateDirection(window);
			bullets.push_back(ammo);
		}
	
}
#include "Drone.h"

Drone::Drone() {}
Drone::~Drone() {}
Drone::Drone(sf::Vector2f spawn) {
	sf::Image image;
	if (!image.loadFromFile("assets/pics/link.jpg")) {
		cout << "Could not open image: assets/pics/drone.png" << endl;
	}
	image.loadFromFile("assets/pics/drone.png");
	image.createMaskFromColor(sf::Color::White);
	this->tex.loadFromImage(image);

	this->setSize(sf::Vector2f(64, 64));
	this->setOrigin(32, 32);
	this->setPosition(spawn);
	this->setTexture(&tex);
	this->setRotation(0);
	this->damage = 5;
	this->score = 10;
	this->health = 30;
	this->velocity = sf::Vector2f(2.f, 2.f);
	this->hasRockets = false;
}

float Drone::calculateRotation(sf::RenderWindow &window, Player &player) {
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

void Drone::update(sf::RenderWindow &window, Player &player, vector<Enemy *> &enemies, vector<Bullet> &bullets) {
	for (int i = 0; i < enemies.size(); i++) {
		enemies[i]->calculateRotation(window, player);
		enemies[i]->setPosition(enemies[i]->getPosition() + sf::Vector2f(cos(enemies[i]->calculateRotation(window, player)) * enemies[i]->velocity.x, sin(enemies[i]->calculateRotation(window, player)) * enemies[i]->velocity.y));
		enemies[i]->shoot(window, player, enemies, bullets);
		enemies[i]->draw(window);
	}
}
void Drone::shoot(sf::RenderWindow &window, Player &player, vector <Enemy *> &enemies, vector <Bullet> &bullets) {

		int random = rand() % 50 + 1;
		if (random == 1) {
			Bullet ammo(this->getPosition(), player.getPosition(), sf::Color::Green, "enemy");
			ammo.velocity = sf::Vector2f(8, 8);
			ammo.setSize(sf::Vector2f(6, 13));
			ammo.damage = this->damage;
			ammo.calculateRotation(window);
			ammo.calculateDirection(window);
			bullets.push_back(ammo);
		}
	
}

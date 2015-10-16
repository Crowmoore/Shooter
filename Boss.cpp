#include "Boss.h"

Boss::Boss() {}
Boss::~Boss() {}
Boss::Boss(string pathName, sf::Texture *tex, sf::Vector2f spawn) {
	this->score = 1000;
	this->health = 10000;
	this->damage = 10;
	this->velocity = sf::Vector2f(-0.5f, 0);
	this->setPosition(spawn);
	this->encounterHasStarted = false;

	this->hardpoint1.setSize(sf::Vector2f(200, 40));
	this->hardpoint1.setFillColor(sf::Color::Black);
	this->hardpoint1.setOrigin(this->hardpoint1.getSize().x, this->hardpoint1.getSize().y / 2);

	this->hardpoint2.setSize(sf::Vector2f(200, 40));
	this->hardpoint2.setFillColor(sf::Color::Black);
	this->hardpoint2.setOrigin(this->hardpoint2.getSize().x, this->hardpoint2.getSize().y / 2);
}
void Boss::draw(sf::RenderWindow &window) {
	window.draw(*this);
}
float Boss::getHealth() {
	return this->health;
}
float Boss::calculateRotation(sf::RenderWindow &window, Player &player, sf::Vector2f current) {
	sf::Vector2f currentPosition = current;
	sf::Vector2f playerPosition = player.getPosition();
	//cursorPosition = sf::Vector2i(window.mapPixelToCoords(cursorPosition, view));
	const float pi = 3.14159265;

	float dx = playerPosition.x - currentPosition.x;
	float dy = playerPosition.y - currentPosition.y;

	float degrees = (atan2(dy, dx)) * 180.0 / pi;
	float radians = (atan2(dy, dx));

	return degrees + 180;
}
#include "Player.h"

Player::Player() {}
Player::~Player() {}

Player::Player(string imgName, sf::Texture *tex) {
	sf::Image image;
	if (!image.loadFromFile(imgName)) {
		cout << "Could not open image: " << imgName << endl;
	}
	image.loadFromFile(imgName);
	tex->loadFromImage(image);
	tex->setSmooth(true);


	this->setSize(sf::Vector2f(64, 64));
	this->setOrigin(32, 32);
	this->spawnPoint = sf::Vector2f(400, 500);
	this->setPosition(sf::Vector2f(spawnPoint));
	this->setTexture(tex);
	this->health = 100;
	this->damage = 10;
	this->score = 0;
	this->ammoDescription = "Red Rays of Happiness";
	this->shieldCharge = 200;
	this->isShielded = false;
	this->setRotation(0);
	this->acceleration = .2f;
	this->maxVelocity = 5.f;
	this->velocity = sf::Vector2f(0, 0);
	this->alive = true;
}
void Player::draw(sf::RenderWindow &window) {
	window.draw(*this);
}
void Player::lookAtCursor(sf::RenderWindow &window, sf::View &view) {
	sf::Vector2f currentPosition = this->getPosition();
	sf::Vector2i cursorPosition = sf::Mouse::getPosition(window);
	//cursorPosition = sf::Vector2i(window.mapPixelToCoords(cursorPosition, view));
	const float pi = 3.14159265;

	float dx = cursorPosition.x - currentPosition.x;
	float dy = cursorPosition.y - currentPosition.y;

	float rotation = (atan2(dy, dx)) * 180.0 / pi;

	//cout << "Mouse x: " << cursorPosition.x << "Mouse y: " << cursorPosition.y << "Rotation: " << rotation << endl;
	this->setRotation(rotation + 90);
}
void Player::adjustVelocity() {
	if (this->velocity.x < -this->maxVelocity) {
		this->velocity.x = -this->maxVelocity;
	}
	else if (this->velocity.x > this->maxVelocity) {
		this->velocity.x = this->maxVelocity;
	}
	else if (this->velocity.y < -this->maxVelocity) {
		this->velocity.y = -this->maxVelocity;
	}
	else if (this->velocity.y > this->maxVelocity) {
		this->velocity.y = this->maxVelocity;
	}
}
void Player::setAmmoDescription(string descript) {
	this->ammoDescription = descript;
}
void Player::setShieldCharge(float charge) {
	this->shieldCharge = charge;
}
string Player::getAmmoDescription() {
	return this->ammoDescription;
}
void Player::checkBounds(Player &player, float top, float width, float height, float left) {
	if (player.getPosition().y < top + player.getSize().x || player.getPosition().y > height - player.getSize().x) {
		player.velocity.y = 0;
	}
	else if (player.getPosition().x > width - player.getSize().x || player.getPosition().x < left + player.getSize().x) {
		player.velocity.x = 0;
	}
}

float Player::getShieldCharge()
{
	return this->shieldCharge;
}

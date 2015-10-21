#include "Player.h"

Player::Player() {}
Player::~Player() {}

Player::Player(string imgName, sf::Texture *tex, sf::Texture *shieldTex) {
	sf::Image image;
	if (!image.loadFromFile(imgName)) {
		cout << "Could not open image: " << imgName << endl;
	}
	image.loadFromFile(imgName);
	tex->loadFromImage(image);
	tex->setSmooth(true);

	this->shieldMeterBlack.setSize(sf::Vector2f(200, 20));
	this->shieldMeterBlack.setPosition(220, 890);
	this->shieldMeterBlack.setOutlineThickness(1);
	this->shieldMeterBlack.setOutlineColor(sf::Color::White);
	this->shieldMeterBlack.setFillColor(sf::Color::Black);
	
	this->shieldMeterBlue.setPosition(220, 890);
	this->shieldMeterBlue.setOutlineThickness(1);
	this->shieldMeterBlue.setOutlineColor(sf::Color::White);
	this->shieldMeterBlue.setFillColor(sf::Color(51, 153, 255, 255));
	
	shieldTex->loadFromFile("assets/pics/shield.png");
	shieldTex->setSmooth(true);
	this->shield.setTexture(*shieldTex);
	this->shield.setScale(sf::Vector2f(1.5, 1.5));

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
	this->isDying = false;
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

	this->setRotation(rotation + 90);
}
void Player::drawShieldMeter(sf::RenderWindow &window) {
	this->shieldMeterBlue.setSize(sf::Vector2f(this->getShieldCharge(), 20));
	window.draw(this->shieldMeterBlack);
	window.draw(this->shieldMeterBlue);
	
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
void Player::updatePlayer(Player &player, float top, float width, float height, float left) {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && player.getPosition().y > top + player.getSize().x) {
		player.velocity.y -= player.acceleration;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && player.getPosition().x < width - player.getSize().x) {
		player.velocity.x += player.acceleration;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && player.getPosition().y < height - player.getSize().x) {
		player.velocity.y += player.acceleration;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && player.getPosition().x > left + player.getSize().x) {
		player.velocity.x -= player.acceleration;
	}
}
void Player::activateShield(sf::RenderWindow &window) {
	this->shield.setPosition(this->getPosition().x - 37, this->getPosition().y - 37);
	this->shield.setColor(sf::Color(51, 153, 255, 128));
	if (this->shieldCharge > 0) {
		this->shieldCharge -= 0.5;
		this->isShielded = true;
		window.draw(this->shield);
	}
	else {
		this->shieldCharge = 0;
		this->isShielded = false;
	}

}
void Player::shoot(sf::RenderWindow &window, vector <Bullet> &bullets, sf::Sound &laser) {
	Bullet ammo(this->getPosition(), (sf::Vector2f(sf::Mouse::getPosition())), sf::Color::Red, "player");
	ammo.damage = this->damage;
	ammo.velocity = sf::Vector2f(12, 12);
	ammo.calculateRotation(window);
	ammo.calculateDirection(window);
	laser.play();
	bullets.push_back(ammo);
}

float Player::getShieldCharge()
{
	return this->shieldCharge;
}
void Player::checkHealth(sf::Sound &heartbeat) {
	if (this->health <= 20 && this->isDying == false) {
		heartbeat.setLoop(true);
		heartbeat.play();
		this->isDying = true;
	}
	if (this->health <= 0) {
	this->alive = false;
	this->velocity = sf::Vector2f(0, 0);
	}
}

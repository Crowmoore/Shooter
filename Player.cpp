#include "include/Player.h"


Player::~Player() {}

//Initialize the player.
Player::Player() {
	Loader loader;
	this->tex = loader.loadTexture("assets/pics/player.png");
	this->setTexture(tex);

	this->setTextureRect(sf::IntRect(0, 0, 170, 222));
	this->setOrigin(this->getLocalBounds().width / 2, this->getLocalBounds().height / 2);
	this->setScale(sf::Vector2f(0.3, 0.3));

	this->shieldMeterBlack.setSize(sf::Vector2f(200, 20));
	this->shieldMeterBlack.setPosition(350, 1010);
	this->shieldMeterBlack.setOutlineThickness(1);
	this->shieldMeterBlack.setOutlineColor(sf::Color::White);
	this->shieldMeterBlack.setFillColor(sf::Color::Black);
	
	this->shieldMeterBlue.setPosition(350, 1010);
	this->shieldMeterBlue.setOutlineThickness(1);
	this->shieldMeterBlue.setOutlineColor(sf::Color::White);
	this->shieldMeterBlue.setFillColor(sf::Color(51, 153, 255, 255));
	
	this->shieldTex = loader.loadTexture("assets/pics/shield.png");
	this->shieldTex.setSmooth(true);
	this->shield.setTexture(shieldTex);
	this->shield.setScale(sf::Vector2f(1.5, 1.5));

	this->spawnPoint = sf::Vector2f(400, 500);
	this->setPosition(sf::Vector2f(spawnPoint));
	this->health = 100;
	this->damage = 10;
	this->ammoDescription = "Red Rays of Happiness";
	this->shieldCharge = 200;
	this->isShielded = false;
	this->setRotation(0);
	this->acceleration = .2f;
	this->maxVelocity = 5.f;
	this->velocity = sf::Vector2f(0, 0);
	this->alive = true;
	this->isDying = false;
	this->pointMultiplier = 1;
	this->rateOfFire = 0.2;
	this->frameCount = 0;
}
//Draw the player.
void Player::draw(sf::RenderWindow &window) {
	window.draw(*this);
}
//Loop through the spritesheet.
void Player::animate() {
	this->setTextureRect(sf::IntRect(170 * frameCount, 0, 170, 222));
	frameCount++;
	if (frameCount > 2) {
		frameCount = 0;
	}
}
//Calculates and sets the rotation so the player always looks at the cursor.
void Player::lookAtCursor(sf::RenderWindow &window, sf::View &view) {
	sf::Vector2f currentPosition = this->getPosition();
	sf::Vector2i cursorPosition = sf::Vector2i(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
	const float pi = 3.14159265;

	float dx = cursorPosition.x - currentPosition.x;
	float dy = cursorPosition.y - currentPosition.y;

	float rotation = (atan2(dy, dx)) * 180.0 / pi;

	this->setRotation(rotation + 90);
}
//Draw the meter that shows the shield charge.
void Player::drawShieldMeter(sf::RenderWindow &window) {
	this->shieldMeterBlue.setSize(sf::Vector2f(this->getShieldCharge(), 20));
	window.draw(this->shieldMeterBlack);
	window.draw(this->shieldMeterBlue);
	
}
//Prevent the player from exceeding the maximum velocity.
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
int Player::getHealth()
{
	return this->health;
}
void Player::setHealth(int health)
{
	this->health = health;
}
float Player::getAcceleration()
{
	return this->acceleration;
}
void Player::setAcceleration(float acceleration)
{
	this->acceleration = acceleration;
}
float Player::getRateOfFire()
{
	return this->rateOfFire;
}
void Player::setRateOfFire(float rof)
{
	this->rateOfFire = rof;
}
void Player::setShieldCharge(float charge) {
	this->shieldCharge = charge;
}
float Player::getDamage()
{
	return this->damage;
}
void Player::setDamage(float damage)
{
	this->damage = damage;
}
bool Player::getShielded()
{
	return this->isShielded;
}
void Player::setShielded(bool shielded)
{
	this->isShielded = shielded;
}
bool Player::getAlive()
{
	return this->alive;
}
void Player::setAlive(bool alive)
{
	this->alive = alive;
}
sf::Vector2f Player::getVelocity()
{
	return this->velocity;
}
void Player::setVelocity(sf::Vector2f velocity)
{
	this->velocity = velocity;
}
sf::Vector2f Player::getSpawnPoint()
{
	return this->spawnPoint;
}
void Player::setSpawnPoint(sf::Vector2f spawn)
{
	this->spawnPoint = spawn;
}
int Player::getPointMultiplier()
{
	return this->pointMultiplier;
}
void Player::setPointMultiplier(int multiplier)
{
	this->pointMultiplier = multiplier;
}
string Player::getAmmoDescription() {
	return this->ammoDescription;
}
//Prevent the player from going out of bounds by checking the player's position and setting the velocity to 0 if near the edge.
void Player::checkBounds(Player &player, sf::FloatRect bounds) {
	if (player.getPosition().y < bounds.top + player.getOrigin().x || player.getPosition().y > bounds.height - player.getOrigin().x) {
		player.velocity.y = 0;
	}
	else if (player.getPosition().x > bounds.width - player.getOrigin().x || player.getPosition().x < bounds.left + player.getOrigin().x) {
		player.velocity.x = 0;
	}
}
//Process the player's inputs WASD and mouse buttons.
void Player::update(sf::RenderWindow &window, vector <Bullet *> &bullets, sf::Sound &laser, sf::FloatRect bounds, sf::Clock &fireRateTimer) {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && this->getPosition().y > bounds.top) {
		this->velocity.y -= this->acceleration;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && this->getPosition().x < bounds.width) {
		this->velocity.x += this->acceleration;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && this->getPosition().y < bounds.height) {
		this->velocity.y += this->acceleration;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && this->getPosition().x > bounds.left) {
		this->velocity.x -= this->acceleration;
	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		if (fireRateTimer.getElapsedTime().asSeconds() >= this->rateOfFire && this->alive) {
			this->shoot(window, bullets, laser);
			fireRateTimer.restart();
		}
	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
		this->activateShield(window);
	}
	if (this->shieldCharge >= 200) {
		this->shieldCharge = 200;
	}
}
//Activate the shield and reduce the shield charge every frame by 0.5.
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
//Check the player's ammoDescription, create and initialize bullets accordinly. Push the created bullet to the bullets vector.
void Player::shoot(sf::RenderWindow &window, vector <Bullet *> &bullets, sf::Sound &laser) {
	if(this->ammoDescription == "Red Rays of Happiness") {
		Bullet *bullet = new Bullet(this->getPosition(), (sf::Vector2f) (window.mapPixelToCoords(sf::Mouse::getPosition(window))), sf::Color::Red, "player");
		bullet->setDamage(this->damage);
		bullet->setVelocity(sf::Vector2f(12, 12));
		bullet->setScale(0.3, 0.3);
		bullet->calculateRotation(window);
		bullet->calculateDirection(window);
		laser.play();
		bullets.push_back(bullet);
	}
	if (this->ammoDescription == "Green Beams of Hurting") {
		Bullet *bullet = new Bullet(this->getPosition(), (sf::Vector2f) (window.mapPixelToCoords(sf::Mouse::getPosition(window))), sf::Color::Green, "player");
		bullet->setDamage(this->damage);
		bullet->setVelocity(sf::Vector2f(14, 14));
		bullet->setScale(0.4, 0.4);
		bullet->calculateRotation(window);
		bullet->calculateDirection(window);
		laser.play();
		bullets.push_back(bullet);
	}
}

float Player::getShieldCharge()
{
	return this->shieldCharge;
}
//Check player's health and start playing the heartbeat sound if bellow 20. If at 0 or below, player is dead and it's velocity is set to 0.
void Player::checkHealth(sf::Sound &heartbeat) {
	if (this->health <= 20 && this->isDying == false) {
		heartbeat.setLoop(true);
		heartbeat.play();
		this->isDying = true;
	}
	if (this->health <= 0) {
	this->health = 0;
	this->alive = false;
	this->velocity = sf::Vector2f(0, 0);
	}
}

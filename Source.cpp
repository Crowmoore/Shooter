#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include "Bullet.h"
#include "Player.h"
#include <iostream>
#include <math.h>
#include <vector>
#include "Enemy.h"
#include "Marauder.h"
#include "Drone.h"
#include "GameState.h"
#include "Logics.h"
#include "Powerups.h"
#include "Loader.h"
#include <stdlib.h>

using namespace std;

sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Tear");
sf::View view(sf::FloatRect(0, 0, 1920, 1080));
sf::Texture enemyTex;
Player player;
vector <Bullet> bullets;
vector <Powerups *> powers;
Drone drone;
Marauder marauder;
vector <Enemy *> enemies;
Loader loader;
Logics logics;
GameState gameState;
sf::Sprite bgSprite;
sf::Sprite shield;
int waveCount;
sf::FloatRect getViewBounds(const sf::View &);
sf::FloatRect bounds;
void spawnEnemies(int);
void enemyWaves(int);
void drawHUD(sf::Font);

int main()
{
	srand(time(NULL));

	loader.loadMusic("assets/sounds/Hall_of_the_Mountain_King.ogg");

	sf::SoundBuffer laserBuffer;
	sf::SoundBuffer explosionBuffer;
	sf::SoundBuffer heartBuffer;
	sf::SoundBuffer dingBuffer;
	explosionBuffer.loadFromFile("assets/sounds/explosion.wav");
	laserBuffer.loadFromFile("assets/sounds/laser.wav");
	heartBuffer.loadFromFile("assets/sounds/heartbeat.wav");
	dingBuffer.loadFromFile("assets/sounds/ding.wav");
	
	sf::Sound laser;
	sf::Sound explosion;
	sf::Sound heartbeat;
	sf::Sound ding;
	
	explosion.setBuffer(explosionBuffer);
	explosion.setVolume(40);
	laser.setBuffer(laserBuffer);
	laser.setVolume(10);
	heartbeat.setBuffer(heartBuffer);
	heartbeat.setVolume(90);
	ding.setBuffer(dingBuffer);
	ding.setVolume(50);

	sf::Clock enemySpawnTimer;
	sf::Clock waveTimer;
	sf::Clock fireRateTimer;

	int enemySpawnInterval = 3;
	int waveInterval = 5;
	
	waveCount = 1;
	
	sf::Font font(loader.loadFont("assets/fonts/tahoma.ttf"));

	window.setFramerateLimit(60);

	bounds = getViewBounds(view);

	sf::Texture bgTex(loader.loadTexture("assets/pics/bg_space.jpg"));
	bgSprite.setTexture(bgTex);
	bgSprite.setPosition(0, 0);

	gameState.setGameState(1);

	while (window.isOpen())
	{	
		player.adjustVelocity();
		player.isShielded = false;

		sf::Event event;
		while (window.pollEvent(event))
			switch (event.type) {
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
					window.close();
				}

			}
			window.clear();
			window.setView(view);

			if (gameState.getGameState() == 1) {

			window.draw(bgSprite);

			player.checkBounds(player, bounds);
			player.update(window, bullets, laser, bounds, fireRateTimer);
			player.setPosition(player.getPosition() + player.velocity);
			player.lookAtCursor(window, view);
			player.checkHealth(heartbeat);
			player.draw(window);
	
			logics.updateEnemies(window, player, enemies, bullets);		
			logics.resolveCollisions(enemies, player);
			logics.updatePowerups(window, powers, player, ding);

			if (enemySpawnTimer.getElapsedTime().asSeconds() >= enemySpawnInterval) {
				enemyWaves(waveCount);
				enemySpawnTimer.restart();
			}
			if (waveTimer.getElapsedTime().asSeconds() >= waveInterval) {
				waveTimer.restart();
				waveCount++;
			}
			
			if (bullets.size() != 0) {
				logics.resolveBulletHitsOnPlayer(window, bullets, player);
				logics.destroyOutOfBoundsBullets(bullets, bounds);
				logics.resolveBulletHitsOnEnemy(bullets, enemies, player, explosion, powers);
			}

		}
		drawHUD(font);
		window.display();
	}

	return 0;
}
void spawnEnemies(int direction) {
	int randomSpawnX = rand() % (int)view.getSize().x;
	int randomSpawnY = rand() % (int)view.getSize().y;
	switch (direction) {
	case 1: {
		Drone* newEnemy = new Drone(sf::Vector2f(randomSpawnX, -40));
		enemies.push_back(newEnemy);
		break;
	}
	case 2: {
		Drone* newEnemy = new Drone(sf::Vector2f(view.getSize().x + 40, randomSpawnY));
		enemies.push_back(newEnemy);
		break;
	}
	case 3: {
		Drone* newEnemy = new Drone(sf::Vector2f(randomSpawnX, view.getSize().y + 40));
		enemies.push_back(newEnemy);
		break;
	}
	case 4: {
		Marauder* newEnemy = new Marauder(sf::Vector2f(-40, randomSpawnY));
		enemies.push_back(newEnemy);
		break;
	}
	}

}
sf::FloatRect getViewBounds(const sf::View &view)
{
	sf::FloatRect bounds;
	bounds.left = view.getCenter().x - view.getSize().x / 2.f;
	bounds.top = view.getCenter().y - view.getSize().y / 2.f;
	bounds.width = view.getSize().x;
	bounds.height = view.getSize().y;
	return bounds;
}
void enemyWaves(int waveCount) {
	switch (waveCount) {
	case 1:
		spawnEnemies(2);
		break;
	case 2:
		spawnEnemies(2);
		spawnEnemies(4);
		break;
	case 3:
		spawnEnemies(2);
		spawnEnemies(4);
		spawnEnemies(1);
		break;
	case 4:
		spawnEnemies(2);
		spawnEnemies(4);
		spawnEnemies(1);
		spawnEnemies(3);
		break;
	}
}

void drawHUD(sf::Font font) {
	sf::Text ammoDescription("Ammunition:\n" + player.ammoDescription, font);
	ammoDescription.setPosition(10, bounds.height - 150);
	ammoDescription.setColor(sf::Color::White);
	window.draw(ammoDescription);

	sf::Text hp("Hull integrity: " + to_string(player.health) + "%", font);
	hp.setPosition(bounds.width - 280, 10);
	hp.setColor(sf::Color::White);
	window.draw(hp);

	sf::Text score("Score: " + to_string(player.score), font);
	score.setPosition(10, 10);
	score.setColor(sf::Color::White);
	window.draw(score);

	sf::Text multiplier("Multiplier: " + to_string(player.pointMultiplier), font);
	multiplier.setPosition(10, 50);
	multiplier.setColor(sf::Color::White);
	window.draw(multiplier);

	sf::Text shieldCharge("Shield charge: ", font);
	shieldCharge.setPosition(10, bounds.height - 200);
	shieldCharge.setColor(sf::Color::White);
	window.draw(shieldCharge);

	player.drawShieldMeter(window);

	if (waveCount < 5) {
		sf::Text wave("Wave " + to_string(waveCount), font);
		wave.setPosition(bounds.width / 2 - wave.getLocalBounds().width / 2, 10);
		wave.setColor(sf::Color::White);
		wave.setCharacterSize(45);
		window.draw(wave);
	}
	if (player.health <= 0) {
		sf::Text death("You have died", font);
		death.setColor(sf::Color::White);
		death.setStyle(sf::Text::Bold);
		death.setCharacterSize(60);
		death.setPosition(bounds.width / 2 - death.getLocalBounds().width / 2, bounds.height / 2 - death.getLocalBounds().height / 2);
		window.draw(death);
	}
	
}
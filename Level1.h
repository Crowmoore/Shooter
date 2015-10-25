#pragma once
#include "Screen.h"
#include "Loader.h"
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
#include "Screen.h"
#include <stdlib.h>

class Level1 : public Screen {
public:
	Level1();
	~Level1() {}
	virtual int run(sf::RenderWindow &window);
	void spawnEnemies(int);
	sf::FloatRect getViewBounds(const sf::View view);
	void enemyWaves(int);
	void drawHUD(sf::Font, sf::FloatRect, sf::RenderWindow &window, int waveCount);

	Player player;
	sf::View view;
	vector <Enemy *> enemies;
	vector <Bullet> bullets;
	vector <Powerups *> powerups;
	Loader loader;
	Logics logics;
	GameState gameState;
	sf::Sprite bgSprite;
	int waveCount;
	sf::FloatRect bounds;
};
Level1::Level1() {	

}

int Level1::run(sf::RenderWindow &window) {	
	this->view.reset(sf::FloatRect(0, 0, 1920, 1080));
	this->isRunning = true;
	srand(time(NULL));
	if (!music.openFromFile("assets/sounds/Drums_of_the_Deep.ogg")) {
		cout << "Could not open assets/sounds/Drums_of_the_Deep.ogg" << endl;
	}
	music.openFromFile("assets/sounds/Drums_of_the_Deep.ogg");
	music.setLoop(true);
	music.setVolume(musicVolume);
	music.play();

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
	explosion.setVolume(10);
	laser.setBuffer(laserBuffer);
	laser.setVolume(5);
	heartbeat.setBuffer(heartBuffer);
	heartbeat.setVolume(90);
	ding.setBuffer(dingBuffer);
	ding.setVolume(10);

	sf::Clock enemySpawnTimer;
	sf::Clock waveTimer;
	sf::Clock fireRateTimer;
	sf::Clock statusTimer;
	bool statusTimerIsOn = false;

	int enemySpawnInterval = 3;
	int waveInterval = 15;

	waveCount = 1;
	points = 0;
	player.health = 100;
	player.shieldCharge = 200;
	player.pointMultiplier = 1;
	player.setPosition(player.spawnPoint);

	sf::Font font(loader.loadFont("assets/fonts/space_age.ttf"));

	window.setFramerateLimit(60);

	bounds = getViewBounds(this->view);

	sf::Texture bgTex(loader.loadTexture("assets/pics/bg_space.jpg"));
	bgSprite.setTexture(bgTex);

	gameState.setGameState(1);

	while (this->isRunning)
	{
		
		player.isShielded = false;
		sf::Event event;
		while (window.pollEvent(event))
			switch (event.type) {
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
					music.pause();
					this->isRunning = false;
					return 2;
				}
				break;
			}
		window.clear();
		window.setView(view);

		if (gameState.getGameState() == 1) {
			window.draw(bgSprite);
			player.adjustVelocity();
			player.checkBounds(player, bounds);
			player.update(window, bullets, laser, bounds, fireRateTimer);
			player.setPosition(player.getPosition() + player.velocity);
			player.lookAtCursor(window, view);
			player.checkHealth(heartbeat);
			player.draw(window);

			logics.updateEnemies(window, player, enemies, bullets);
			logics.resolveCollisions(enemies, player);
			logics.updatePowerups(window, powerups, player, ding);

			if (enemySpawnTimer.getElapsedTime().asSeconds() >= enemySpawnInterval && waveCount < 4) {
				this->enemyWaves(waveCount);
				enemySpawnTimer.restart();
			}
			if (waveTimer.getElapsedTime().asSeconds() >= waveInterval && waveCount < 4) {
				waveTimer.restart();
				waveCount++;
			}
			if (waveCount == 4 && enemies.size() == 0) {
				sf::Text victory("Victory!", font);
				victory.setColor(sf::Color::White);
				victory.setStyle(sf::Text::Bold);
				victory.setCharacterSize(60);
				victory.setPosition(bounds.width / 2 - victory.getLocalBounds().width / 2, bounds.height / 2 - victory.getLocalBounds().height / 2);
				window.draw(victory);
				
				if (statusTimerIsOn != true) {
					statusTimer.restart();
					statusTimerIsOn = true;
				}
				if (statusTimer.getElapsedTime().asSeconds() > 3) {
					survivalBonus = player.health * 2;
					if(powerups.size() != 0) {
						powerups.clear();
					}
					if (bullets.size() != 0) {
						bullets.clear();
					}
					return 4;
				}
			}

			if (bullets.size() != 0) {
				logics.resolveBulletHitsOnPlayer(window, bullets, player);
				logics.destroyOutOfBoundsBullets(bullets, bounds);
				logics.resolveBulletHitsOnEnemy(bullets, enemies, player, explosion, powerups);
			}
			this->drawHUD(font, bounds, window, waveCount);
		}

		window.display();
	}
	return -1;

}
void Level1::spawnEnemies(int direction) {
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
sf::FloatRect Level1::getViewBounds(const sf::View view)
{
	sf::FloatRect bounds;
	bounds.left = view.getCenter().x - view.getSize().x / 2.f;
	bounds.top = view.getCenter().y - view.getSize().y / 2.f;
	bounds.width = view.getSize().x;
	bounds.height = view.getSize().y;
	return bounds;
}
void Level1::enemyWaves(int waveCount) {
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

void Level1::drawHUD(sf::Font font, sf::FloatRect bounds, sf::RenderWindow &window, int waveCount) {
	sf::Text ammoDescription("Ammunition:\n" + player.ammoDescription, font);
	ammoDescription.setPosition(10, bounds.height - 150);
	ammoDescription.setColor(sf::Color::White);
	window.draw(ammoDescription);

	sf::Text hp("Hull integrity: " + to_string(player.health) + "%", font);
	hp.setPosition(bounds.width - 450, 10);
	hp.setColor(sf::Color::White);
	window.draw(hp);

	sf::Text score("Score: " + to_string(points), font);
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
		sf::Text death("Mission failed", font);
		death.setColor(sf::Color::White);
		death.setStyle(sf::Text::Bold);
		death.setCharacterSize(60);
		death.setPosition(bounds.width / 2 - death.getLocalBounds().width / 2, bounds.height / 2 - death.getLocalBounds().height / 2);
		window.draw(death);
	}
}
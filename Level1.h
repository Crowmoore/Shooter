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
#include "Explosion.h"
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
	void initPlayer(Player &);

	Player player;
	sf::View view;
	vector <Enemy *> enemies;
	vector <Bullet *> bullets;
	vector <Explosion *> explosions;
	vector <Powerups *> powerups;
	Loader loader;
	Logics logics;
	GameState gameState;
	sf::Sprite bgSprite;
	sf::Texture explosionTex;
	sf::Sprite cursor;
	int waveCount;
	sf::FloatRect bounds;
	sf::Clock statusTimer;
	bool statusTimerIsOn;
};
Level1::Level1() {	

}

int Level1::run(sf::RenderWindow &window) {	
	this->view.reset(sf::FloatRect(0, 0, 1920, 1080));

	loader.loadMusic("assets/sounds/Drums_of_the_Deep.ogg");
	music.setVolume(musicVolume);
	music.play();

	sf::Font font(loader.loadFont("assets/fonts/space_age.ttf"));

	explosionTex = loader.loadTexture("assets/pics/explosion.png");

	sf::Sound laser = loader.loadSound("assets/sounds/laser.wav");
	sf::Sound explosion = loader.loadSound("assets/sounds/explosion.wav");
	sf::Sound heartbeat = loader.loadSound("assets/sounds/heartbeat.wav");
	sf::Sound ding = loader.loadSound("assets/sounds/ding.wav");

	explosion.setVolume(10);
	laser.setVolume(5);
	heartbeat.setVolume(90);
	ding.setVolume(10);

	sf::Clock enemySpawnTimer;
	sf::Clock waveTimer;
	sf::Clock fireRateTimer;
	statusTimerIsOn = false;

	int enemySpawnInterval = 3;
	int waveInterval = 10;

	if (this->isRunning != true) {
		this->initPlayer(player);
		this->waveCount = 1;
	}

	window.setFramerateLimit(60);

	bounds = getViewBounds(this->view);

	sf::Texture bgTex(loader.loadTexture("assets/pics/bg_space.jpg"));
	bgSprite.setTexture(bgTex);

	sf::Texture cursorTex(loader.loadTexture("assets/pics/cursor.png"));
	cursorTex.setSmooth(true);
	cursor.setTexture(cursorTex);
	cursor.setOrigin(cursor.getLocalBounds().width / 2, cursor.getLocalBounds().height / 2);

	sf::Text paused("Paused", font);
	sf::Text resume("Resume", font);
	sf::Text exit("Exit to Main menu", font);
	int selection = 0;

	sf::SoundBuffer clipBuffer;
	clipBuffer.loadFromFile("assets/sounds/clip.wav");
	sf::Sound clip;
	clip.setBuffer(clipBuffer);

	paused.setCharacterSize(90);
	paused.setPosition(view.getSize().x / 2 - paused.getLocalBounds().width / 2, 20);

	resume.setCharacterSize(60);
	resume.setPosition(view.getSize().x / 2 - resume.getLocalBounds().width / 2, 500);

	exit.setCharacterSize(60);
	exit.setPosition(view.getSize().x / 2 - exit.getLocalBounds().width / 2, 600);


	this->isRunning = true;
	gameState.setGameState(1);

	while (this->isRunning)
	{

		player.setShielded(false);
		sf::Event event;
		while (window.pollEvent(event))
			switch (event.type) {
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
					if (gameState.getGameState() == 1) {
						gameState.setGameState(0);
					}
					else {
						gameState.setGameState(1);
					}
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && player.getHealth() == 0) {
					music.pause();
					enemies.clear();
					bullets.clear();
					explosions.clear();
					powerups.clear();
					this->isRunning = false;
					return 0;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && gameState.getGameState() == 0 || sf::Keyboard::isKeyPressed(sf::Keyboard::E) && gameState.getGameState() == 0) {
					if (selection == 0) {
						gameState.setGameState(1);
					}
					else {
						music.pause();
						enemies.clear();
						bullets.clear();
						explosions.clear();
						powerups.clear();
						this->isRunning = false;
						return 0;
					}
					break;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && gameState.getGameState() == 0 || sf::Keyboard::isKeyPressed(sf::Keyboard::W) && gameState.getGameState() == 0) {
					clip.play();
					selection -= 1;
					break;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && gameState.getGameState() == 0 || sf::Keyboard::isKeyPressed(sf::Keyboard::S) && gameState.getGameState() == 0) {
					clip.play();
					selection += 1;
					break;
				}
			}
			if (selection < 0) {
				selection = 1;
			}
			else if (selection > 1) {
				selection = 0;
			}
			if (selection == 0) {
				resume.setColor(sf::Color(255, 0, 0, 255));
				exit.setColor(sf::Color(255, 255, 255, 255));
			}
			else {
				resume.setColor(sf::Color(255, 255, 255, 255));
				exit.setColor(sf::Color(255, 0, 0, 255));
			}
		if (gameState.getGameState() == 0) {
			window.clear();
			window.draw(paused);
			window.draw(resume);
			window.draw(exit);
			window.display();
		}

		if (gameState.getGameState() == 1) {
			window.clear();
			window.setView(view);
			window.draw(bgSprite);
			player.adjustVelocity();
			player.checkBounds(player, bounds);
			player.update(window, bullets, laser, bounds, fireRateTimer);
			player.animate();
			player.setPosition(player.getPosition() + player.getVelocity());
			player.lookAtCursor(window, view);
			player.checkHealth(heartbeat);
			player.draw(window);

			logics.updateEnemies(window, player, enemies, bullets);
			logics.resolveCollisions(enemies, player);
			logics.updatePowerups(window, powerups, player, ding);

			cursor.setPosition((sf::Vector2f)(window.mapPixelToCoords(sf::Mouse::getPosition(window))));

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
				victory.setCharacterSize(100);
				victory.setPosition(bounds.width / 2 - victory.getLocalBounds().width / 2, bounds.height / 2 - victory.getLocalBounds().height / 2);
				window.draw(victory);
				
				if (statusTimerIsOn != true) {
					statusTimer.restart();
					statusTimerIsOn = true;
				}
				if (statusTimer.getElapsedTime().asSeconds() > 3) {
					survivalBonus = player.getHealth() * 2;
					powerups.clear();
					bullets.clear();
					this->isRunning = false;
					return 3;
				}
			}
			if (player.getAlive() != true) {
				sf::Text death("Mission failed", font);
				death.setColor(sf::Color::White);
				death.setStyle(sf::Text::Bold);
				death.setCharacterSize(100);
				death.setPosition(bounds.width / 2 - death.getLocalBounds().width / 2, bounds.height / 2 - death.getLocalBounds().height / 2);
				window.draw(death);
				if (statusTimerIsOn != true) {
					statusTimer.restart();
					statusTimerIsOn = true;
				}
				if (statusTimer.getElapsedTime().asSeconds() > 3) {
					powerups.clear();
					enemies.clear();
					explosions.clear();
					bullets.clear();
					music.pause();
					this->isRunning = false;
					return 0;
				}
			}
			

			if (bullets.size() != 0) {
				logics.resolveBulletHitsOnPlayer(window, bullets, player, explosion, explosions, &explosionTex);
				logics.destroyOutOfBoundsBullets(bullets, bounds);
				logics.resolveBulletHitsOnEnemy(window, bullets, enemies, player, explosion, powerups, explosions, &explosionTex);
				
			}
			logics.updateExplosions(window, explosions);
		}
		if (gameState.getGameState() == 1) {
			this->drawHUD(font, bounds, window, waveCount);
			window.display();
		}

	}
	return -1;

}
void Level1::spawnEnemies(int direction) {
	int randomSpawnX = rand() % (int)view.getSize().x;
	int randomSpawnY = rand() % (int)view.getSize().y;
	int randomEnemy = rand() % 5 + 1;
	switch (direction) {
	case 1: {
		if (randomEnemy == 1) {
			Marauder* newEnemy = new Marauder(sf::Vector2f(randomSpawnX, -40));
			enemies.push_back(newEnemy);
		}
		else {
			Drone* newEnemy = new Drone(sf::Vector2f(randomSpawnX, -40));
			enemies.push_back(newEnemy);
		}
		break;
	}
	case 2: {
		if (randomEnemy == 1) {
			Marauder* newEnemy = new Marauder(sf::Vector2f(view.getSize().x + 40, randomSpawnY));
			enemies.push_back(newEnemy);
		}
		else {
			Drone* newEnemy = new Drone(sf::Vector2f(view.getSize().x + 40, randomSpawnY));
			enemies.push_back(newEnemy);
		}
		break;
	}
	case 3: {
		if (randomEnemy == 1) {
			Marauder* newEnemy = new Marauder(sf::Vector2f(randomSpawnX, view.getSize().y + 40));
			enemies.push_back(newEnemy);
		}
		else {
			Drone* newEnemy = new Drone(sf::Vector2f(randomSpawnX, view.getSize().y + 40));
			enemies.push_back(newEnemy);
		}
		break;
	}
	case 4: {
		if (randomEnemy == 1) {
			Marauder* newEnemy = new Marauder(sf::Vector2f(-40, randomSpawnY));
			enemies.push_back(newEnemy);
		}
		else {
			Drone* newEnemy = new Drone(sf::Vector2f(-40, randomSpawnY));
			enemies.push_back(newEnemy);
		}
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
	sf::Text ammoDescription("Ammunition:\n" + player.getAmmoDescription(), font);
	ammoDescription.setPosition(10, bounds.height - 150);
	ammoDescription.setColor(sf::Color::White);
	window.draw(ammoDescription);

	sf::Text hp("Hull integrity: " + to_string(player.getHealth()) + "%", font);
	hp.setPosition(bounds.width - 450, 10);
	hp.setColor(sf::Color::White);
	window.draw(hp);

	window.draw(cursor);

	sf::Text score("Score: " + to_string(points), font);
	score.setPosition(10, 10);
	score.setColor(sf::Color::White);
	window.draw(score);

	sf::Text multiplier("Multiplier: " + to_string(player.getPointMultiplier()), font);
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
}
void Level1::initPlayer(Player &player) {
	points = 0;
	player.setAlive(true);
	player.setHealth(100);
	player.setShieldCharge(200);
	player.setPointMultiplier(1);
	player.setPosition(player.getSpawnPoint());
	player.setVelocity(sf::Vector2f(0, 0));
	player.setAmmoDescription("Red Rays of Happiness");
	player.setRateOfFire(0.2);
}
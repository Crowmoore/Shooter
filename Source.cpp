#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include "Bullet.h"
#include "Player.h"
#include <iostream>
#include <math.h>
#include "WorldPosition.h"
#include <vector>
#include "Enemy.h"
#include "Boss.h"
#include "GameState.h"
#include "Logics.h"
#include "Loader.h"
#include <stdlib.h>

using namespace std;

sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Tear");
sf::View view(sf::FloatRect(0, 0, 1920, 1080));
sf::Texture playerTex;
sf::Texture shieldTex;
sf::Texture enemyTex;
sf::Texture bossTex;
Player player("assets/pics/ship.png", &playerTex, &shieldTex);
vector <Bullet> bullets;
vector <Enemy> enemies;
Enemy enemy;
Loader loader;
Logics logics;
GameState gameState;
sf::Sprite bgSprite;
sf::Sprite shield;

sf::FloatRect getViewBounds(const sf::View &);
sf::FloatRect bounds;
void spawnEnemies(int);
void enemyWaves(int);
void drawHUD(sf::Font, int);
sf::Text bossWarning;
Boss boss("assets/pics/boss.png", &bossTex, sf::Vector2f(2000, 0));
sf::RectangleShape meterBlue;
sf::RectangleShape meterBlack;
sf::RectangleShape bossHealth;

int main()
{
	srand(time(NULL));

	int cannonFire = 0;

	loader.loadMusic("assets/sounds/Hall_of_the_Mountain_King.ogg");

	sf::SoundBuffer laserBuffer;
	sf::SoundBuffer explosionBuffer;
	sf::SoundBuffer heartBuffer;
	explosionBuffer.loadFromFile("assets/sounds/explosion.wav");
	laserBuffer.loadFromFile("assets/sounds/laser.wav");
	heartBuffer.loadFromFile("assets/sounds/heartbeat.wav");
	
	sf::Sound laser;
	sf::Sound explosion;
	sf::Sound heartbeat;
	
	explosion.setBuffer(explosionBuffer);
	explosion.setVolume(40);
	laser.setBuffer(laserBuffer);
	laser.setVolume(10);
	heartbeat.setBuffer(heartBuffer);
	heartbeat.setVolume(70);

	bossWarning.setPosition(2000, 800);
	sf::Clock enemySpawnTimer;
	sf::Clock waveTimer;
	int enemySpawnInterval = 3;

	int waveInterval = 19;
	int waveCount = 1;
	sf::Font font(loader.loadFont("assets/fonts/tahoma.ttf"));

	window.setFramerateLimit(60);

	bounds = getViewBounds(view);

	sf::Texture bgTex(loader.loadTexture("assets/pics/bg_space.jpg"));
	bgSprite.setTexture(bgTex);
	bgSprite.setPosition(0, 0);

	sf::Texture hardpointTex;
	hardpointTex.loadFromFile("assets/pics/cannonsheet.png");
	hardpointTex.setSmooth(true);
	sf::Sprite hardpoint1(hardpointTex);
	hardpoint1.setTextureRect(sf::IntRect(0, 0, 1000, 200));
	hardpoint1.setScale(sf::Vector2f(0.3, 0.3));
	hardpoint1.setOrigin(sf::Vector2f(hardpoint1.getTextureRect().width, hardpoint1.getTextureRect().height));

	sf::Sprite hardpoint2(hardpointTex);
	hardpoint2.setTextureRect(sf::IntRect(0, 0, 1000, 200));
	hardpoint2.setScale(sf::Vector2f(0.3, 0.3));
	hardpoint2.setOrigin(sf::Vector2f(hardpoint2.getTextureRect().width, hardpoint2.getTextureRect().height));


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


				player.checkBounds(player, bounds.top, bounds.width, bounds.height, bounds.left);
				player.updatePlayer(player, bounds.top, bounds.width, bounds.height, bounds.left);
				player.setPosition(player.getPosition() + player.velocity);
				player.lookAtCursor(window, view);
				player.checkHealth(heartbeat);
				player.draw(window);

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				player.shoot(window, bullets, laser);
			}
			if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
				player.activateShield(window);

			}
			


			enemy.updateEnemies(window, player, enemies, bullets);

			if (enemySpawnTimer.getElapsedTime().asSeconds() >= enemySpawnInterval) {
				enemyWaves(waveCount);
				enemySpawnTimer.restart();
			}
			if (waveTimer.getElapsedTime().asSeconds() >= waveInterval) {
				waveTimer.restart();
				waveCount++;
			}

			if (waveCount >= 5) {
				boss.setRadius(500);		
				boss.setFillColor(sf::Color::Cyan);
				hardpoint1.setPosition(boss.getPosition().x + 400, boss.getPosition().y + 300);
				hardpoint2.setPosition(boss.getPosition().x + 400, boss.getPosition().y + 600);
				if (boss.getPosition().x > 1300) {
					boss.move(boss.velocity);			
				}
				else if (boss.getPosition().x <= 1300) {
					sf::Text bossName("Capital ship", font);
					bossName.setPosition(500, 10);
					bossName.setColor(sf::Color::White);
					window.draw(bossName);

					bossHealth.setSize(sf::Vector2f(boss.getHealth() / 10, 20));
					
					bossHealth.setPosition(500, 60);
					bossHealth.setFillColor(sf::Color::Red);
					boss.encounterHasStarted = true;
					window.draw(bossHealth);
				}
				boss.draw(window);
				hardpoint1.setRotation(boss.calculateRotation(window, player, hardpoint1.getPosition()));
				hardpoint2.setRotation(boss.calculateRotation(window, player, hardpoint2.getPosition()));
				window.draw(hardpoint1);
				window.draw(hardpoint2);

				int random = rand() % 25 + 1;
				if (random == 25 && boss.encounterHasStarted == true) {
					sf::Vector2f point;
					if (hardpoint2.getRotation() < 180) {
						point = sf::Vector2f(hardpoint2.getGlobalBounds().left, hardpoint2.getGlobalBounds().top + 20);
					}
					else {
						point = sf::Vector2f(hardpoint2.getGlobalBounds().left, hardpoint2.getGlobalBounds().top + hardpoint2.getGlobalBounds().height - 20);
					}
					hardpoint2.setTextureRect(sf::IntRect(0, cannonFire * 195.5, 1000, 200));
					
					Bullet ammo(point, player.getPosition(), sf::Color::Yellow, "enemy");
					ammo.velocity = sf::Vector2f(8, 8);

					ammo.damage = boss.damage;
					ammo.setSize(sf::Vector2f(10, 25));
					ammo.calculateRotation(window);
					ammo.calculateDirection(window);
					bullets.push_back(ammo);
				}
				else if (random == 1 && boss.encounterHasStarted == true) {
					sf::Vector2f point;
					if (hardpoint1.getRotation() < 180) {
						point = sf::Vector2f(hardpoint1.getGlobalBounds().left, hardpoint1.getGlobalBounds().top + 20);
					}
					else {
						point = sf::Vector2f(hardpoint1.getGlobalBounds().left, hardpoint1.getGlobalBounds().top + hardpoint1.getGlobalBounds().height - 20);
					}
					hardpoint1.setTextureRect(sf::IntRect(0, cannonFire * 195.5, 1000, 200));
					Bullet ammo(point, player.getPosition(), sf::Color::Yellow, "enemy");
					ammo.velocity = sf::Vector2f(8, 8);

					ammo.damage = boss.damage;
					ammo.setSize(sf::Vector2f(10, 25));
					ammo.calculateRotation(window);
					ammo.calculateDirection(window);
					bullets.push_back(ammo);
				}
				cannonFire++;
					if (cannonFire == 8) {
						cannonFire = 0;
					}
			}
			

			

			
			if (bullets.size() != 0) {
				logics.resolveBulletHitsOnPlayer(window, bullets, player);
				logics.destroyOutOfBoundsBullets(bullets, bounds.top, bounds.width, bounds.height, bounds.left);
				logics.resolveBulletHitsOnEnemy(bullets, enemies, player, explosion);
				for (int bullet = 0; bullet < bullets.size(); bullet++) {
					if (bullets[bullet].id == "player" && bullets[bullet].getGlobalBounds().intersects(hardpoint1.getGlobalBounds()) && boss.encounterHasStarted == true
						|| bullets[bullet].id == "player" && bullets[bullet].getGlobalBounds().intersects(hardpoint2.getGlobalBounds()) && boss.encounterHasStarted == true) {
						boss.health -= bullets[bullet].damage;
						bullets.erase(bullets.begin() + bullet);
						if (boss.health <= 0) {
							boss.health = 0;
							player.score += boss.score;
							
					}
				}

			}
		}

	}
	
		


		if (waveCount >= 5 && bossWarning.getPosition().x > -1100) {
			bossWarning.setString("WARNING! Capital ship approaching!");
			bossWarning.setFont(font);
			bossWarning.setColor(sf::Color::White);
			bossWarning.setStyle(sf::Text::Bold);
			bossWarning.setCharacterSize(60);
			bossWarning.move(-5, 0);
			window.draw(bossWarning);
		}
		

		
		drawHUD(font, waveCount);
		
		
		window.display();
	}

	return 0;
}
void spawnEnemies(int direction) {
	int randomSpawnX = rand() % (int)view.getSize().x;
	int randomSpawnY = rand() % (int)view.getSize().y;
	switch (direction) {
	case 1: {
		Enemy newEnemy("assets/pics/link.jpg", &enemyTex, sf::Vector2f(randomSpawnX, -40));
		enemies.push_back(newEnemy);
		break;
	}
	case 2: {
		Enemy newEnemy("assets/pics/link.jpg", &enemyTex, sf::Vector2f(view.getSize().x + 40, randomSpawnY));
		enemies.push_back(newEnemy);
		break;
	}
	case 3: {
		Enemy newEnemy("assets/pics/link.jpg", &enemyTex, sf::Vector2f(randomSpawnX, view.getSize().y + 40));
		enemies.push_back(newEnemy);
		break;
	}
	case 4: {
		Enemy newEnemy("assets/pics/link.jpg", &enemyTex, sf::Vector2f(-40, randomSpawnY));
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

void drawHUD(sf::Font font, int waveCount) {
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

	sf::Text shieldCharge("Shield charge: ", font);
	shieldCharge.setPosition(10, bounds.height - 200);
	shieldCharge.setColor(sf::Color::White);
	window.draw(shieldCharge);

	player.drawShieldMeter(window);

	if (waveCount < 5) {
		sf::Text wave("Wave " + to_string(waveCount), font);
		wave.setPosition(bounds.width / 2, 10);
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
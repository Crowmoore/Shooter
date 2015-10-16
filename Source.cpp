#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include "Player.h"
#include <iostream>
#include <math.h>
#include "WorldPosition.h"
#include "Ammunition.h"
#include <vector>
#include "Enemy.h"
#include "Boss.h"
#include "GameState.h"
#include "Logics.h"
#include <stdlib.h>

using namespace std;

sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Tear", sf::Style::Fullscreen);
sf::View view(sf::FloatRect(0, 0, 1920, 1080));
sf::Texture playerTex;
sf::Texture enemyTex;
sf::Texture bossTex;
Player player("assets/pics/ship.png", &playerTex);
Logics logics;
Enemy enemy;
GameState gameState;
sf::Sprite bgSprite;
sf::Sprite shield;
vector <Ammunition> bullets;
vector <Ammunition> enemyBullets;
vector <Enemy> enemies;
void checkEnemyCollision();
sf::FloatRect getViewBounds(const sf::View &);
sf::FloatRect bounds;
void spawnEnemies(int);
void enemyWaves(int);
void updateEnemies(sf::RenderWindow &, Player);
void drawHUD(sf::Font, int);
sf::Text bossWarning;
Boss boss("assets/pics/boss.png", &bossTex, sf::Vector2f(2000, 0));
sf::RectangleShape meterBlue;
sf::RectangleShape meterBlack;
sf::RectangleShape bossHealth;
sf::CircleShape explosion;

int main()
{
	srand(time(NULL));

	int cannonFire = 0;

	sf::Music music;
	if (!music.openFromFile("assets/sounds/Hall_of_the_Mountain_King.ogg")) {
		cout << "Could not open assets/sounds/Hall_of_the_Mountain_King.ogg" << endl;
	}
	music.setVolume(50);
	music.play();

	bossWarning.setPosition(2000, 800);
	sf::Clock warningTimer;
	sf::Clock enemySpawnTimer;
	sf::Clock waveTimer;
	int enemySpawnInterval = 3;

	int waveInterval = 19;
	int waveCount = 1;
	sf::Font font;

	explosion.setRadius(25);
	explosion.setFillColor(sf::Color(255, 128, 0, 200));

	if (!font.loadFromFile("assets/fonts/tahoma.ttf")) {
		cout << "Could not load font: assets/fonts/tahoma.ttf" << endl;
	}

	window.setFramerateLimit(60);

	bounds = getViewBounds(view);

	sf::Image bg;
	if (!bg.loadFromFile("assets/pics/bg_space.jpg")) {
		cout << "Could not open image: assets/pics/bg_space.jpg" << endl;
		return -1;
	}
	else {
		bg.loadFromFile("assets/pics/bg_space.jpg");
	}
	sf::Texture bgTex;
	bgTex.loadFromImage(bg);
	bgSprite.setTexture(bgTex);
	bgSprite.setPosition(0, 0);

	sf::Image shieldImg;
	shieldImg.loadFromFile("assets/pics/shield.png");
	sf::Texture shieldTex;
	shieldTex.loadFromImage(shieldImg);
	shieldTex.setSmooth(true);
	shield.setTexture(shieldTex);
	shield.setScale(sf::Vector2f(1.5, 1.5));

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
		
		player.checkBounds(player, bounds.top, bounds.width, bounds.height, bounds.left);

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && player.getPosition().y > bounds.top + player.getSize().x) {
				player.velocity.y -= player.acceleration;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && player.getPosition().x < bounds.width - player.getSize().x) {
				player.velocity.x += player.acceleration;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && player.getPosition().y < bounds.height - player.getSize().x) {
				player.velocity.y += player.acceleration;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && player.getPosition().x > bounds.left + player.getSize().x) {
				player.velocity.x -= player.acceleration;
			}

			window.clear();
			window.setView(view);

			if (gameState.getGameState() == 1) {
				
				window.draw(bgSprite);
				player.setPosition(player.getPosition() + player.velocity);
				player.lookAtCursor(window, view);
				player.draw(window);

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				Ammunition ammo(player.getPosition(), (sf::Vector2f(sf::Mouse::getPosition())), sf::Color::Red, "player");
				ammo.damage = player.damage;
				ammo.velocity = sf::Vector2f(12, 12);
				ammo.calculateRotation(window);
				ammo.calculateDirection(window);
				bullets.push_back(ammo);
			}
			if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
				shield.setPosition(player.getPosition().x - 37, player.getPosition().y - 37);
				shield.setColor(sf::Color(51, 153, 255, 128));
				if (player.shieldCharge > 0) {
					player.shieldCharge -= 0.5;
					player.isShielded = true;
					window.draw(shield);
				}
				else {
					player.shieldCharge = 0;
					player.isShielded = false;
				}

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
					
					Ammunition ammo(point, player.getPosition(), sf::Color::Yellow, "enemy");
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
					Ammunition ammo(point, player.getPosition(), sf::Color::Yellow, "enemy");
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
				for (int bullet = 0; bullet < bullets.size(); bullet++) {
					bullets[bullet].setPosition(bullets[bullet].getPosition() + bullets[bullet].velocity);
					bullets[bullet].draw(window);
					if (bullets[bullet].getPosition().x < bounds.left
						|| bullets[bullet].getPosition().x > bounds.width 
						|| bullets[bullet].getPosition().y < bounds.top 
						|| bullets[bullet].getPosition().y > bounds.height) {
						bullets.erase(bullets.begin() + bullet);
					}
					else if (bullets[bullet].id == "enemy" && bullets[bullet].getGlobalBounds().intersects(player.getGlobalBounds())) {
						if(player.isShielded != true) {
							player.health -= bullets[bullet].damage;
						}
						else {
							shield.setColor(sf::Color(255, 255, 255, 128));
							window.draw(shield);
						}
						bullets.erase(bullets.begin() + bullet);
					}
				}
				if (bullets.size() != 0) {
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
						for (int enemy = 0; enemy < enemies.size(); enemy++) {
							if (bullets[bullet].id == "player" && bullets[bullet].getGlobalBounds().intersects(enemies[enemy].getGlobalBounds())) {
								enemies[enemy].health -= bullets[bullet].damage;
								bullets.erase(bullets.begin() + bullet);
								break;
							}
							if (enemies[enemy].health <= 0) {
								player.score += enemies[enemy].score;
								explosion.setPosition(enemies[enemy].getPosition().x - 16, enemies[enemy].getPosition().y - 16);
								window.draw(explosion);
								enemies.erase(enemies.begin() + enemy);
							}
							
						}
					}

				}
			}

		}
		
		/*if (player.health <= 0) {
			player.alive = false;
			gameState.setGameState(0);
		}*/


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

	meterBlack.setSize(sf::Vector2f(200, 20));
	meterBlack.setPosition(220, bounds.height - 190);
	meterBlack.setOutlineThickness(1);
	meterBlack.setOutlineColor(sf::Color::White);
	meterBlack.setFillColor(sf::Color::Black);
	window.draw(meterBlack);

	meterBlue.setSize(sf::Vector2f(player.getShieldCharge(), 20));
	meterBlue.setPosition(220, bounds.height - 190);
	meterBlue.setOutlineThickness(1);
	meterBlue.setOutlineColor(sf::Color::White);
	meterBlue.setFillColor(sf::Color(51, 153, 255, 255));
	window.draw(meterBlue);

	if (waveCount < 5) {
		sf::Text wave("Wave " + to_string(waveCount), font);
		wave.setPosition(bounds.width / 2, 10);
		wave.setColor(sf::Color::White);
		wave.setCharacterSize(45);
		window.draw(wave);
	}
	
}
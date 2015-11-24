#pragma once
#include "resources.h"

class Level2 : public Screen {
public:
	Level2();
	~Level2() {}
	virtual int run(sf::RenderWindow &window);
	void spawnEnemies(int);
	sf::FloatRect getViewBounds(const sf::View view);
	void enemyWaves(int);
	void drawHUD(sf::Font, sf::FloatRect, sf::RenderWindow &window, int waveCount);
	void initPlayer(Player &);
	void initLevel();
	void drawPauseScreen(sf::RenderWindow &);
	void pauseGame();
	void endLevel();
	void missionFailed(sf::RenderWindow &);
	void missionSuccesfull(sf::RenderWindow &);

private:
	Player player;
	Loader loader;
	Logics logics;
	GameState gameState;
	sf::View view;
	sf::Font font;
	sf::Sound clip;
	sf::Sound laser;
	sf::Sound explosion;
	sf::Sound heartbeat;
	sf::Sound ding;
	sf::Texture bgTex;
	sf::Texture explosionTex;
	sf::Texture cursorTex;
	sf::Sprite bgSprite;
	sf::Sprite cursor;
	sf::Clock enemySpawnTimer;
	sf::Clock waveTimer;
	sf::Clock fireRateTimer;
	sf::Clock statusTimer;
	int enemySpawnInterval;
	int waveInterval;
	int waveCount;
	int selection;
	bool statusTimerIsOn;
	sf::Text death;
	sf::Text victory;
	sf::FloatRect bounds;
	vector <Enemy *> enemies;
	vector <Bullet *> bullets;
	vector <Explosion *> explosions;
	vector <Powerups *> powerups;
	vector <Missile *> missiles;
};
Level2::Level2() {}

int Level2::run(sf::RenderWindow &window) {

	initLevel();
	window.setFramerateLimit(60);


	this->isRunning = true;
	gameState.setGameState(1);
	//Start the main game loop
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
				//Pause the game
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
					pauseGame();
				}
				//Pause menu controls
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && gameState.getGameState() == 0 || sf::Keyboard::isKeyPressed(sf::Keyboard::E) && gameState.getGameState() == 0) {
					//Resume the game
					if (selection == 0) {
						gameState.setGameState(1);
					}
					//Exit to main menu
					else {
						endLevel();
						return 0;
					}
					break;
				}
				//Select pause menu options
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
		if (gameState.getGameState() == 1) {
			window.clear();
			window.setView(view);
			window.draw(bgSprite);

			//Update everything player related
			cursor.setPosition((sf::Vector2f)(window.mapPixelToCoords(sf::Mouse::getPosition(window))));
			player.adjustVelocity();
			player.checkBounds(player, bounds);
			player.update(window, bullets, laser, bounds, fireRateTimer);
			player.animate();
			player.setPosition(player.getPosition() + player.getVelocity());
			player.lookAtCursor(window, view);
			player.checkHealth(heartbeat);
			player.draw(window);

			//Update everything else on the screen
			logics.updateEnemies(window, player, enemies, bullets, missiles);
			logics.resolveCollisions(enemies, player);
			logics.updatePowerups(window, powerups, player, ding);
			logics.updateExplosions(window, explosions);
			logics.updateMissiles(window, player, missiles, bullets);
			logics.resolveBulletHitsOnPlayer(window, bullets, player, explosion, explosions, &explosionTex);
			logics.destroyOutOfBoundsBullets(bullets, bounds);
			logics.resolveBulletHitsOnEnemy(window, bullets, enemies, player, explosion, powerups, explosions, &explosionTex);

			//Check if it's time to spawn a new enemy
			if (enemySpawnTimer.getElapsedTime().asSeconds() >= enemySpawnInterval && waveCount < 10) {
				this->enemyWaves(waveCount);
				enemySpawnTimer.restart();
			}
			//Check if it's time to move to a next enemy wave
			if (waveTimer.getElapsedTime().asSeconds() >= waveInterval && waveCount < 10) {
				waveTimer.restart();
				waveCount++;
			}
			//Check is the player has survived all waves and cleared all enemies. End level with a success
			if (waveCount == 10 && enemies.size() == 0) {
				missionSuccesfull(window);
				if (statusTimer.getElapsedTime().asSeconds() > 3) {
					survivalBonus = player.getHealth() * 2;
					heartbeat.stop();
					endLevel();
					return 4;
				}
			}
			//If the player has died, end level in failure.
			if (player.getAlive() != true) {
				missionFailed(window);
				if (statusTimer.getElapsedTime().asSeconds() > 3) {
					heartbeat.stop();
					endLevel();
					return 0;
				}
			}
			this->drawHUD(font, bounds, window, waveCount);
			window.display();
		}
		//if gamestate is 0, draw the pause screen
		else {
			window.clear();
			drawPauseScreen(window);
			window.display();
		}
	}
	//This is here just in case. We should never find ourselves here
	return -1;

}
//Spawn random enemies from random locations on a specific side of the screen
void Level2::spawnEnemies(int direction) {
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
//Draw the mission successfull notification
void Level2::missionSuccesfull(sf::RenderWindow &window) {
	victory.setString("Victory!");
	victory.setFont(font);
	victory.setColor(sf::Color::White);
	victory.setStyle(sf::Text::Bold);
	victory.setCharacterSize(100);
	victory.setPosition(bounds.width / 2 - victory.getLocalBounds().width / 2, bounds.height / 2 - victory.getLocalBounds().height / 2);
	window.draw(victory);

	if (statusTimerIsOn != true) {
		statusTimer.restart();
		statusTimerIsOn = true;
	}
}
//Draw the mission failed notification
void Level2::missionFailed(sf::RenderWindow &window) {
	death.setString("Mission failed");
	death.setFont(font);
	death.setColor(sf::Color::White);
	death.setStyle(sf::Text::Bold);
	death.setCharacterSize(100);
	death.setPosition(bounds.width / 2 - death.getLocalBounds().width / 2, bounds.height / 2 - death.getLocalBounds().height / 2);
	window.draw(death);
	if (statusTimerIsOn != true) {
		statusTimer.restart();
		statusTimerIsOn = true;
	}
}
//Get the view borders as a sf::FloatRect
sf::FloatRect Level2::getViewBounds(const sf::View view)
{
	sf::FloatRect bounds;
	bounds.left = view.getCenter().x - view.getSize().x / 2.f;
	bounds.top = view.getCenter().y - view.getSize().y / 2.f;
	bounds.width = view.getSize().x;
	bounds.height = view.getSize().y;
	return bounds;
}
//End the level by clearing everything
void Level2::endLevel() {
	music.pause();
	enemies.clear();
	bullets.clear();
	explosions.clear();
	powerups.clear();
	missiles.clear();
	this->isRunning = false;
}
//Pause/unpause the game
void Level2::pauseGame() {
	if (gameState.getGameState() == 1) {
		gameState.setGameState(0);
	}
	else {
		gameState.setGameState(1);
	}
}
//Spawn enemies based on wavecount. Different cases determine where the enemies will spawn
void Level2::enemyWaves(int waveCount) {
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
	default:
		spawnEnemies(2);
		spawnEnemies(4);
		spawnEnemies(1);
		spawnEnemies(3);
		break;
	}
	
}
//Draw the head-up display
void Level2::drawHUD(sf::Font font, sf::FloatRect bounds, sf::RenderWindow &window, int waveCount) {
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

	if (waveCount <= 10) {
		sf::Text wave("Wave " + to_string(waveCount), font);
		wave.setPosition(bounds.width / 2 - wave.getLocalBounds().width / 2, 10);
		wave.setColor(sf::Color::White);
		wave.setCharacterSize(45);
		window.draw(wave);
	}
}
//Initialize/reset the player
void Level2::initPlayer(Player &player) {
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
//Draw the pause screen
void Level2::drawPauseScreen(sf::RenderWindow &window) {
	sf::Text paused("Paused", font);
	sf::Text resume("Resume", font);
	sf::Text exit("Exit", font);

	paused.setCharacterSize(90);
	paused.setPosition(view.getSize().x / 2 - paused.getLocalBounds().width / 2, 20);

	resume.setCharacterSize(60);
	resume.setPosition(view.getSize().x / 2 - resume.getLocalBounds().width / 2, 500);

	exit.setCharacterSize(60);
	exit.setPosition(view.getSize().x / 2 - exit.getLocalBounds().width / 2, 600);

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

	window.draw(paused);
	window.draw(resume);
	window.draw(exit);
}
//Initialize the level and load assets
void Level2::initLevel() {
	this->view.reset(sf::FloatRect(0, 0, sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height));
	currentLevel = 2;

	loader.loadMusic("assets/sounds/Phantom_From_Space.ogg");
	music.setVolume(musicVolume);
	music.play();

	laser = loader.loadSound("assets/sounds/laser.wav");
	explosion = loader.loadSound("assets/sounds/explosion.wav");
	heartbeat = loader.loadSound("assets/sounds/heartbeat.wav");
	ding = loader.loadSound("assets/sounds/ding.wav");
	clip = loader.loadSound("assets/sounds/clip.wav");

	explosion.setVolume(10);
	laser.setVolume(5);
	heartbeat.setVolume(90);
	ding.setVolume(10);

	bgTex = loader.loadTexture("assets/pics/blue_space.jpg");
	bgSprite.setTexture(bgTex);

	font = loader.loadFont("assets/fonts/space_age.ttf");

	explosionTex = loader.loadTexture("assets/pics/explosion_small.png");

	cursorTex = loader.loadTexture("assets/pics/cursor.png");
	cursorTex.setSmooth(true);
	cursor.setTexture(cursorTex);
	cursor.setOrigin(cursor.getLocalBounds().width / 2, cursor.getLocalBounds().height / 2);

	bounds = getViewBounds(this->view);

	statusTimerIsOn = false;

	enemySpawnInterval = 5;
	waveInterval = 10;

	int selection = 0;

	if (this->isRunning != true) {
		this->initPlayer(player);
		this->waveCount = 1;
	}


}
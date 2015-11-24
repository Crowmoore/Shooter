#include "include/Logics.h"

Logics::Logics() {}
Logics::~Logics() {}

//Animate and update all the enemies
void Logics::updateEnemies(sf::RenderWindow &window, Player &player, vector<Enemy *> &enemies, vector<Bullet *> &bullets, vector <Missile *> &missiles) {
	for (int i = 0; i < enemies.size(); i++) {
		enemies[i]->animate();
		enemies[i]->calculateRotation(window, player);
		enemies[i]->setPosition(enemies[i]->getPosition() + sf::Vector2f(cos(enemies[i]->calculateRotation(window, player)) * enemies[i]->getVelocity().x, sin(enemies[i]->calculateRotation(window, player)) * enemies[i]->getVelocity().y));
		enemies[i]->shoot(window, player, enemies, bullets, missiles);
		enemies[i]->draw(window);
	}
}
//Check is the player collides with an enemy or if 2 enemies collide with each other and resolve the collision by moving the enemy
void Logics::resolveCollisions(vector <Enemy *> & enemies, Player &player) {
	for (int i = 0; i < enemies.size(); i++) {
		if (player.getGlobalBounds().intersects(enemies[i]->getGlobalBounds())) {
			enemies[i]->move((enemies[i]->getPosition().x / 32) - (player.getPosition().x / 32), (enemies[i]->getPosition().y / 32) - (player.getPosition().y / 32));
		}
		for (int j = i + 1; j < enemies.size(); j++) {
			if (enemies[i]->getGlobalBounds().intersects(enemies[j]->getGlobalBounds())) {
				enemies[i]->move((enemies[i]->getPosition().x / 64) - (enemies[j]->getPosition().x / 64), (enemies[i]->getPosition().y / 64) - (enemies[j]->getPosition().y / 64));
			}

		}
	}
}
//Check if an enemy bullet hits the player.
void Logics::resolveBulletHitsOnPlayer(sf::RenderWindow &window,
										vector <Bullet *> &bullets,
										Player &player,
										sf::Sound &explosionClip,
										vector <Explosion *> &explosions,
										sf::Texture *explosionTex) {
	for (int i = 0; i < bullets.size(); i++) {
		bullets[i]->setPosition(bullets[i]->getPosition() + bullets[i]->getVelocity());
		bullets[i]->draw(window);
		if (bullets[i]->getId() == "enemy" && bullets[i]->getGlobalBounds().intersects(player.getGlobalBounds())) {
			//If the player is not shielded reduce player's health by bullet damage amount.
			if (player.getShielded() != true) {
				player.setHealth(player.getHealth() - bullets[i]->getDamage());
			}
			else {
				player.shield.setColor(sf::Color(255, 255, 255, 128));
				window.draw(player.shield);
			}
			//If the player is dead, every bullet that hits creates an explosion.
			if (player.getAlive() != true) {
				Explosion *explosion = new Explosion(*explosionTex);
				explosion->setPosition(player.getPosition());
				explosions.push_back(explosion);
				explosionClip.play();
			}
			delete bullets[i];
			bullets.erase(bullets.begin() + i);
		}
	}
}
//Update missiles and check collisions with the player and with the player's bullets.
void Logics::updateMissiles(sf::RenderWindow &window, Player &player, vector <Missile *> &missiles, vector <Bullet *> &bullets) {
	for (int j = 0; j < missiles.size(); j++) {
		missiles[j]->calculateRotation(window, player);
		missiles[j]->setPosition(missiles[j]->getPosition() + sf::Vector2f(cos(missiles[j]->calculateRotation(window, player)) * missiles[j]->velocity.x, sin(missiles[j]->calculateRotation(window, player)) * missiles[j]->velocity.y));
		missiles[j]->draw(window);
		if (missiles[j]->getGlobalBounds().intersects(player.getGlobalBounds())) {			
			if (player.getShielded() != true) {
				player.setHealth(player.getHealth() - missiles[j]->damage);
			}
			else {
				player.shield.setColor(sf::Color(255, 255, 255, 128));
				window.draw(player.shield);
			}
			delete missiles[j];
			missiles.erase(missiles.begin() + j);
			break;
		}
		for (int i = 0; i < bullets.size(); i++) {
			if (bullets[i]->getId() == "player" && bullets[i]->getGlobalBounds().intersects(missiles[j]->getGlobalBounds())) {
				extern int points;
				points += missiles[j]->score * player.getPointMultiplier();
				delete missiles[j];
				missiles.erase(missiles.begin() + j);
				delete bullets[i];
				bullets.erase(bullets.begin() + i);
				break;
			}

		}
	}
}
//Check if a bullet is out of bounds and destroy if it is.
void Logics::destroyOutOfBoundsBullets(vector <Bullet *> &bullets, sf::FloatRect bounds) {
	for (int i = 0; i < bullets.size(); i++) {
		if (bullets[i]->getPosition().x < bounds.left
			|| bullets[i]->getPosition().x > bounds.width
			|| bullets[i]->getPosition().y < bounds.top
			|| bullets[i]->getPosition().y > bounds.height) {
			delete bullets[i];
			bullets.erase(bullets.begin() + i);
		}
	}
}
//Check if the player's bullet hits an enemy, check enemies health and destroy if 0 or under.
void Logics::resolveBulletHitsOnEnemy(sf::RenderWindow &window,
										vector <Bullet *> &bullets,
										vector <Enemy *> &enemies,
										Player &player,
										sf::Sound &explosionClip,
										vector <Powerups *> &powerups,
										vector <Explosion *> &explosions,
										sf::Texture *explosionTex) {
	for (int i = 0; i < bullets.size(); i++) {
		for (int j = 0; j < enemies.size(); j++) {
			if (bullets[i]->getId() == "player" && bullets[i]->getGlobalBounds().intersects(enemies[j]->getGlobalBounds())) {
				enemies[j]->setHealth(enemies[j]->getHealth() - bullets[i]->getDamage());
				delete bullets[i];
				bullets.erase(bullets.begin() + i);
				break;
			}
			//If enemy's health is 0 or less, add it's points to the global point pool, incerement global kill count by one
			//and create a new explosion. Check if a new Power-Up should be spawned then delete the enemy.
			if (enemies[j]->getHealth() <= 0) {
				extern int points;
				points += enemies[j]->getScore() * player.getPointMultiplier();
				extern int enemiesKilled;
				enemiesKilled++;
				Explosion *explosion = new Explosion(*explosionTex);
				explosion->setPosition(enemies[j]->getPosition());
				explosions.push_back(explosion);
				explosionClip.play();
				int random = rand() % 5 + 1;
				if (random == 1) {
					Powerups *powerup = this->spawnRandomPowerUp();
					powerup->setPosition(enemies[j]->getPosition());
					powerups.push_back(powerup);
				}
				delete enemies[j];
				enemies.erase(enemies.begin() + j);
				}
		}
	}
}
//Cycle through the explosions list, animate and destroy.
void Logics::updateExplosions(sf::RenderWindow &window, vector <Explosion *> &explosions) {
	for (int i = 0; i < explosions.size(); i++) {		
		explosions[i]->explode(window);
		if (explosions[i]->getExploded() != false) {
			delete explosions[i];
			explosions.erase(explosions.begin() + i);
		}
	}
}
//Check if the player picks up a Power-Up, set it's effect on player and destroy.
void Logics::updatePowerups(sf::RenderWindow &window, vector <Powerups *> &powerups, Player &player, sf::Sound &ding) {
	for (int i = 0; i < powerups.size(); i++) {
		powerups[i]->move(rand() % 3 + (-1), rand() % 3 + (-1));
		powerups[i]->draw(window);
		if (powerups[i]->getId() == "multiplier" && powerups[i]->getGlobalBounds().intersects(player.getGlobalBounds())) {
			ding.play();
			player.setPointMultiplier(player.getPointMultiplier() * 2);
			delete powerups[i];
			powerups.erase(powerups.begin() + i);
			break;
		}
		if (powerups[i]->getId() == "30shield" && powerups[i]->getGlobalBounds().intersects(player.getGlobalBounds())) {
			ding.play();
			player.setShieldCharge(player.getShieldCharge() + 30);
			delete powerups[i];
			powerups.erase(powerups.begin() + i);
			break;
		}
		if (powerups[i]->getId() == "firerateUp" && powerups[i]->getGlobalBounds().intersects(player.getGlobalBounds())) {
			ding.play();
			player.setRateOfFire(player.getRateOfFire() - 0.1);
			delete powerups[i];
			powerups.erase(powerups.begin() + i);
			break;
		}
		if (powerups[i]->getId() == "greenBullets" && powerups[i]->getGlobalBounds().intersects(player.getGlobalBounds())) {
			ding.play();
			player.setAmmoDescription("Green Beams of Hurting");
			player.setDamage(30);
			player.setRateOfFire(0.4);
			delete powerups[i];
			powerups.erase(powerups.begin() + i);
			break;
		}
	}
}
//Roll a number between 1-4 and spawn a Power-Up based on the roll.
Powerups* Logics::spawnRandomPowerUp() {
	int random = rand() % 4 + 1;
	switch (random) {
	case 1: {
		Powerups *powerup = new Powerups("assets/pics/2x.png", "multiplier");
		return powerup;
		break;
	}
	case 2: {
		Powerups *powerup = new Powerups("assets/pics/30shield.png", "30shield");
		return powerup;
		break;
	}
	case 3: {
		Powerups *powerup = new Powerups("assets/pics/firerate_up.png", "firerateUp");
		return powerup;
		break;
	}
	case 4: {
		Powerups *powerup = new Powerups("assets/pics/green_bullets.png", "greenBullets");
		return powerup;
		break;
	}
	default:
		break;
	}
}
#include "Logics.h"

Logics::Logics() {}
Logics::~Logics() {}

void Logics::updateEnemies(sf::RenderWindow &window, Player &player, vector<Enemy *> &enemies, vector<Bullet *> &bullets) {
	for (int i = 0; i < enemies.size(); i++) {
		enemies[i]->animate();
		enemies[i]->calculateRotation(window, player);
		enemies[i]->setPosition(enemies[i]->getPosition() + sf::Vector2f(cos(enemies[i]->calculateRotation(window, player)) * enemies[i]->velocity.x, sin(enemies[i]->calculateRotation(window, player)) * enemies[i]->velocity.y));
		enemies[i]->shoot(window, player, enemies, bullets);
		enemies[i]->draw(window);
	}
}
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
void Logics::resolveBulletHitsOnPlayer(sf::RenderWindow &window, vector <Bullet *> &bullets, Player &player) {
	for (int bullet = 0; bullet < bullets.size(); bullet++) {
		bullets[bullet]->setPosition(bullets[bullet]->getPosition() + bullets[bullet]->velocity);
		bullets[bullet]->draw(window);
		if (bullets[bullet]->id == "enemy" && bullets[bullet]->getGlobalBounds().intersects(player.getGlobalBounds())) {
			if (player.isShielded != true) {
				player.health -= bullets[bullet]->damage;
			}
			else {
				player.shield.setColor(sf::Color(255, 255, 255, 128));
				window.draw(player.shield);
			}
			delete bullets[bullet];
			bullets.erase(bullets.begin() + bullet);
		}
	}
}
void Logics::destroyOutOfBoundsBullets(vector <Bullet *> &bullets, sf::FloatRect bounds) {
	for (int bullet = 0; bullet < bullets.size(); bullet++) {
		if (bullets[bullet]->getPosition().x < bounds.left
			|| bullets[bullet]->getPosition().x > bounds.width
			|| bullets[bullet]->getPosition().y < bounds.top
			|| bullets[bullet]->getPosition().y > bounds.height) {
			delete bullets[bullet];
			bullets.erase(bullets.begin() + bullet);
		}
	}
}
void Logics::resolveBulletHitsOnEnemy(sf::RenderWindow &window, vector <Bullet *> &bullets, vector <Enemy *> &enemies, Player &player, sf::Sound &explosionClip, vector <Powerups *> &powerups, vector <Explosion *> &explosions, sf::Texture *explosionTex) {
	for (int bullet = 0; bullet < bullets.size(); bullet++) {
		for (int enemy = 0; enemy < enemies.size(); enemy++) {
			if (bullets[bullet]->id == "player" && bullets[bullet]->getGlobalBounds().intersects(enemies[enemy]->getGlobalBounds())) {
				enemies[enemy]->health -= bullets[bullet]->damage;
				delete bullets[bullet];
				bullets.erase(bullets.begin() + bullet);
				break;
			}
			if (enemies[enemy]->health <= 0) {
				extern int points;
				points += enemies[enemy]->score * player.pointMultiplier;
				extern int enemiesKilled;
				enemiesKilled++;
				Explosion *explosion = new Explosion(*explosionTex);
				explosion->setPosition(enemies[enemy]->getPosition());
				explosions.push_back(explosion);
				explosionClip.play();
				int random = rand() % 5 + 1;
				if (random == 1) {
					Powerups *powerup = this->spawnRandomPowerUp();
					powerup->setPosition(enemies[enemy]->getPosition());
					powerups.push_back(powerup);
				}
				delete enemies[enemy];
				enemies.erase(enemies.begin() + enemy);
				}
		}
	}
}
void Logics::updateExplosions(sf::RenderWindow &window, vector <Explosion *> &explosions) {
	for (int i = 0; i < explosions.size(); i++) {		
		explosions[i]->explode(window);
		if (explosions[i]->hasExploded != false) {
			delete explosions[i];
			explosions.erase(explosions.begin() + i);
		}
	}
}

void Logics::updatePowerups(sf::RenderWindow &window, vector <Powerups *> &powerups, Player &player, sf::Sound &ding) {
	for (int i = 0; i < powerups.size(); i++) {
		powerups[i]->move(rand() % 3 + (-1), rand() % 3 + (-1));
		powerups[i]->draw(window);
		if (powerups[i]->id == "multiplier" && powerups[i]->getGlobalBounds().intersects(player.getGlobalBounds())) {
			ding.play();
			player.pointMultiplier = player.pointMultiplier * 2;
			delete powerups[i];
			powerups.erase(powerups.begin() + i);
			break;
		}
		if (powerups[i]->id == "30shield" && powerups[i]->getGlobalBounds().intersects(player.getGlobalBounds())) {
			ding.play();
			player.shieldCharge += 30;
			delete powerups[i];
			powerups.erase(powerups.begin() + i);
			break;
		}
		if (powerups[i]->id == "firerateUp" && powerups[i]->getGlobalBounds().intersects(player.getGlobalBounds())) {
			ding.play();
			player.rateOfFire -= 0.1;
			delete powerups[i];
			powerups.erase(powerups.begin() + i);
			break;
		}
		if (powerups[i]->id == "greenBullets" && powerups[i]->getGlobalBounds().intersects(player.getGlobalBounds())) {
			ding.play();
			player.ammoDescription = "Green Beams of Hurting";
			player.damage += 10;
			player.rateOfFire += 0.2;
			delete powerups[i];
			powerups.erase(powerups.begin() + i);
			break;
		}
	}
}
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
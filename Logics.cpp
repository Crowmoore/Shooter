#include "Logics.h"

Logics::Logics() {}
Logics::~Logics() {}

/*void Logics::resolveCollision(vector <Enemy> & enemies, Player &player) {
	for (int i = 0; i < enemies.size(); i++) {
		if (player.getGlobalBounds().intersects(enemies[i].getGlobalBounds())) {
			enemies[i].move((enemies[i].getSize().x / 8) - (player.getOrigin().x / 64), (enemies[i].getSize().y / 8) - (player.getOrigin().y / 64));
		}
		for (int j = i + 1; j < enemies.size(); j++) {
			if (enemies[i].getGlobalBounds().intersects(enemies[j].getGlobalBounds())) {
				enemies[i].move((enemies[i].getSize().x / 16) - (enemies[j].getOrigin().x / 32), (enemies[i].getSize().y / 16) - (enemies[j].getOrigin().y / 32));
			}

		}
	}
}*/
void Logics::resolveBulletHitsOnPlayer(sf::RenderWindow &window, vector <Bullet> &bullets, Player &player) {
	for (int bullet = 0; bullet < bullets.size(); bullet++) {
		bullets[bullet].setPosition(bullets[bullet].getPosition() + bullets[bullet].velocity);
		bullets[bullet].draw(window);
		if (bullets[bullet].id == "enemy" && bullets[bullet].getGlobalBounds().intersects(player.getGlobalBounds())) {
			if (player.isShielded != true) {
				player.health -= bullets[bullet].damage;
			}
			else {
				player.shield.setColor(sf::Color(255, 255, 255, 128));
				window.draw(player.shield);
			}
			bullets.erase(bullets.begin() + bullet);
		}
	}
}
void Logics::destroyOutOfBoundsBullets(vector <Bullet> &bullets, float top, float width, float height, float left) {
	for (int bullet = 0; bullet < bullets.size(); bullet++) {
		if (bullets[bullet].getPosition().x < left
			|| bullets[bullet].getPosition().x > width
			|| bullets[bullet].getPosition().y < top
			|| bullets[bullet].getPosition().y > height) {
			bullets.erase(bullets.begin() + bullet);
		}
	}
}
void Logics::resolveBulletHitsOnEnemy(vector <Bullet> &bullets, vector <Enemy> &enemies, Player &player, sf::Sound &explosion) {
	for (int bullet = 0; bullet < bullets.size(); bullet++) {
		for (int enemy = 0; enemy < enemies.size(); enemy++) {
			if (bullets[bullet].id == "player" && bullets[bullet].getGlobalBounds().intersects(enemies[enemy].getGlobalBounds())) {
				enemies[enemy].health -= bullets[bullet].damage;
				bullets.erase(bullets.begin() + bullet);
				break;
			}
			if (enemies[enemy].health <= 0) {
				player.score += enemies[enemy].score;
				explosion.play();
				enemies.erase(enemies.begin() + enemy);
				}
		}
	}
}
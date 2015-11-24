#pragma once
#include "Screen.h"
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include "Loader.h"

class PointsScreen : public Screen {
public:
	PointsScreen();
	~PointsScreen() {}
	virtual int run(sf::RenderWindow &window);
private:
	sf::View view;
	sf::Event event;
	Loader loader;
	sf::Font font;
	int survivalCount;
	int pointsCount;
	int killCount;
	int totalCount;
	bool isRunning;
	bool pointsCounted;
	bool highscoreSaved;
	int totalPoints;
	sf::Sound pointSound;
};
PointsScreen::PointsScreen() {
}
int PointsScreen::run(sf::RenderWindow &window) {
	
	this->view.reset(sf::FloatRect(0, 0, sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height));
	
	this->isRunning = true;
	font = loader.loadFont("assets/fonts/space_age.ttf");
	sf::Text missionSuccessfull("Mission successfull", font);
	this->survivalCount = 0;
	this->pointsCount = 0;
	this->killCount = 0;
	this->totalCount = 0;
	this->pointsCounted = false;
	this->highscoreSaved = false;	
	this->totalPoints = points + survivalBonus;
	
	sf::Text promt("Press enter to continue", font);

	pointSound = loader.loadSound("assets/sounds/points.wav");
	pointSound.setVolume(10);

	missionSuccessfull.setCharacterSize(60);
	missionSuccessfull.setPosition(view.getSize().x / 2 - missionSuccessfull.getLocalBounds().width / 2, 20);
	
	promt.setCharacterSize(50);
	promt.setPosition(view.getSize().x / 2 - promt.getLocalBounds().width / 2, 900);

	while (isRunning) {
		while (window.pollEvent(event))
			switch (event.type) {
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				//Pressing Enter or E displays total points if points are not yet counted and exits to main menu if they are.
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) || sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
					if (this->pointsCounted == false) {
						this->pointsCount = points;
						this->killCount = enemiesKilled;
						this->survivalCount = survivalBonus;
						this->totalCount = totalPoints;
					}
					else {
						music.pause();
						return 0;
					}
				}
			}
		if (this->pointsCount < points) {
			this->pointSound.play();
			this->pointsCount++;
		}
		sf::Text score("Points: " + to_string(this->pointsCount), font);
		score.setCharacterSize(50);
		score.setPosition(200, 300);

		if (this->killCount < enemiesKilled && this->pointsCount == points) {
			this->pointSound.play();
			this->killCount++;
		}
		sf::Text kills("Enemies killed: " + to_string(this->killCount), font);
		kills.setCharacterSize(50);
		kills.setPosition(200, 400);

		if (this->survivalCount < survivalBonus && this->killCount == enemiesKilled) {
			this->pointSound.play();
			this->survivalCount++;
		}
		sf::Text survival("Survival bonus: " + to_string(this->survivalCount), font);
		survival.setCharacterSize(50);
		survival.setPosition(200, 500);

		if (this->totalCount < totalPoints && this->survivalCount == survivalBonus) {
			this->pointSound.play();
			this->totalCount++;
		}
		if (this->totalCount == this->totalPoints) {
			this->pointsCounted = true;
		}
		//Save level specific highscores
		if (currentLevel == 1) {
			if (this->totalPoints > highscores[0] && this->pointsCounted != false && this->highscoreSaved != true) {
				highscores[0] = this->totalPoints;
				loader.saveHighscoreToFile(highscores);
				this->highscoreSaved = true;
			}
		}
		else {
			if (this->totalPoints > highscores[1] && this->pointsCounted != false && this->highscoreSaved != true) {
				highscores[1] = this->totalPoints;
				loader.saveHighscoreToFile(highscores);
				this->highscoreSaved = true;
			}
		}
		sf::Text total("Total: " + to_string(totalCount), font);
		total.setCharacterSize(50);
		total.setPosition(200, 600);

		window.clear();
		window.draw(missionSuccessfull);
		window.draw(score);
		window.draw(kills);
		window.draw(survival);
		window.draw(total);
		window.draw(promt);
		window.display();
	}
	return -1;
}
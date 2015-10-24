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

	sf::View view;
};
PointsScreen::PointsScreen() {
}
int PointsScreen::run(sf::RenderWindow &window) {
	sf::Event event;
	this->view.reset(sf::FloatRect(0, 0, 1920, 1080));
	Loader loader;
	bool isRunning = true;
	sf::Font font(loader.loadFont("assets/fonts/space_age.ttf"));
	sf::Text missionSuccessfull("Mission successfull", font);
	int calculateSurvival = 0;
	int calculatePoints = 0;
	int calculateKills = 0;
	int calculateTotal = 0;
	
	int totalPoints = points + survivalBonus;
	
	sf::Text promt("Press enter to continue", font);

	sf::SoundBuffer pointsBuffer;
	pointsBuffer.loadFromFile("assets/sounds/points.wav");
	sf::Sound pointSound;
	pointSound.setBuffer(pointsBuffer);
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
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
					return 0;
				}
			}
		if (calculatePoints < points) {
			pointSound.play();
			calculatePoints++;
		}
		sf::Text score("Points: " + to_string(calculatePoints), font);
		score.setCharacterSize(50);
		score.setPosition(200, 300);
		if (calculateKills < enemiesKilled && calculatePoints == points) {
			pointSound.play();
			calculateKills++;
		}
		sf::Text kills("Enemies killed: " + to_string(calculateKills), font);
		kills.setCharacterSize(50);
		kills.setPosition(200, 400);
		if (calculateSurvival < survivalBonus && calculateKills == enemiesKilled) {
			pointSound.play();
			calculateSurvival++;
		}
		sf::Text survival("Survival bonus: " + to_string(calculateSurvival), font);
		survival.setCharacterSize(50);
		survival.setPosition(200, 500);
		if (calculateTotal < totalPoints && calculateSurvival == survivalBonus) {
			pointSound.play();
			calculateTotal++;
		}
		sf::Text total("Total: " + to_string(calculateTotal), font);
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
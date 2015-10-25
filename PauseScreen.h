#pragma once
#include "Screen.h"
#include <SFML\Graphics.hpp>
#include "Loader.h"

class PauseScreen : public Screen {
public:
	PauseScreen();
	~PauseScreen() {}
	virtual int run(sf::RenderWindow &window);

	sf::View view;
};
PauseScreen::PauseScreen() {
}

int PauseScreen::run(sf::RenderWindow &window) {
	sf::Event event;
	this->view.reset(sf::FloatRect(0, 0, 1920, 1080));
	Loader loader;
	bool isRunning = true;
	sf::Font font(loader.loadFont("assets/fonts/space_age.ttf"));
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

	while (isRunning) {
		while (window.pollEvent(event))
			switch (event.type) {
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
					return 1;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
					clip.play();
					selection -= 1;
					break;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
					clip.play();
					selection += 1;
					break;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
					if (selection == 0) {
						return 1;
					}
					else {
						return 0;
					}
					break;
				}
			default:
				break;
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
		window.clear();
		window.setView(view);
		window.draw(paused);
		window.draw(resume);
		window.draw(exit);
		window.display();
	}
	return -1;
}

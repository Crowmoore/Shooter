#pragma once
#include "Screen.h"
#include <SFML\Graphics.hpp>
#include "Loader.h"

class OptionsScreen : public Screen {
public:
	OptionsScreen();
	~OptionsScreen() {}
	virtual int run(sf::RenderWindow &window);

	sf::View view;
};
OptionsScreen::OptionsScreen() {
}

int OptionsScreen::run(sf::RenderWindow &window) {
	sf::Event event;
	this->view.reset(sf::FloatRect(0, 0, 1920, 1080));
	Loader loader;
	bool isWindowed = true;
	bool isRunning = true;
	sf::Texture bgTex(loader.loadTexture("assets/pics/outer_space.jpg"));
	sf::Sprite bgSprite;
	bgSprite.setTexture(bgTex);
	bgSprite.setPosition(0, 0);
	sf::Font font(loader.loadFont("assets/fonts/space_age.ttf"));
	sf::Text back("Back", font);
	sf::Text display("Display", font);
	sf::Text volume("Music volume", font);
	sf::Text windowed("Windowed", font);
	sf::Text fullscreen("Fullscreen", font);
	int selection = 0;
	int displaySelection = 0;

	sf::SoundBuffer clipBuffer;
	clipBuffer.loadFromFile("assets/sounds/clip.wav");
	sf::Sound clip;
	clip.setBuffer(clipBuffer);

	volume.setCharacterSize(60);
	volume.setPosition(200, 300);

	display.setCharacterSize(60);
	display.setPosition(200, 400);

	windowed.setCharacterSize(60);
	windowed.setPosition(1200, 400);

	fullscreen.setCharacterSize(60);
	fullscreen.setPosition(1200, 400);

	back.setCharacterSize(60);
	back.setPosition(200, 500);

		sf::RectangleShape volumeMeterBlack;
	sf::RectangleShape volumeMeterRed;
	volumeMeterBlack.setSize(sf::Vector2f(300, 40));
	volumeMeterBlack.setPosition(1200, 320);
	volumeMeterBlack.setOutlineThickness(1);
	volumeMeterBlack.setOutlineColor(sf::Color::White);
	volumeMeterBlack.setFillColor(sf::Color::Black);

	volumeMeterRed.setPosition(1200, 320);
	volumeMeterRed.setOutlineThickness(1);
	volumeMeterRed.setOutlineColor(sf::Color::White);
	volumeMeterRed.setFillColor(sf::Color::Red);

	while (isRunning) {
		while (window.pollEvent(event))
			switch (event.type) {
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
					return 0;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
					clip.play();
					selection -= 1;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
					clip.play();
					selection += 1;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
					if (selection == 0) {

					}
					else if (selection == 1) {
						if (displaySelection == 0) {
							window.create(sf::VideoMode::getDesktopMode(), "Project T.E.A.R");
							isWindowed = true;
						}
						else {
							window.create(sf::VideoMode::getDesktopMode(), "Project T.E.A.R", sf::Style::Fullscreen);
							isWindowed = false;
						}
					} else {
						return 0;
					}
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && selection == 0 && musicVolume > 0) {
					clip.play();
					musicVolume -= 10;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && selection == 0 && musicVolume < 100) {
					clip.play();
					musicVolume += 10;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && selection == 1 || sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && selection == 1) {
					clip.play();
					if (displaySelection == 0) {
						displaySelection = 1;
					}
					else {
						displaySelection = 0;
					}
				}
				break;
			default:
				break;
			}
		volumeMeterRed.setSize(sf::Vector2f(musicVolume * 3, 40));
		if (selection < 0) {
			selection = 2;
		}
		else if (selection > 2) {
			selection = 0;
		}
		switch (selection) {
		case 0:
			volume.setColor(sf::Color(255, 0, 0, 255));
			display.setColor(sf::Color(255, 255, 255, 255));
			back.setColor(sf::Color(255, 255, 255, 255));
			break;
		case 1:
			volume.setColor(sf::Color(255, 255, 255, 255));
			display.setColor(sf::Color(255, 0, 0, 255));
			back.setColor(sf::Color(255, 255, 255, 255));
			break;
		case 2:
			volume.setColor(sf::Color(255, 255, 255, 255));
			display.setColor(sf::Color(255, 255, 255, 255));
			back.setColor(sf::Color(255, 0, 0, 255));
			break;
		}


		window.clear();
		window.draw(bgSprite);
		window.setView(view);
		window.draw(volume);
		window.draw(display);
		window.draw(back);
		window.draw(volumeMeterBlack);
		window.draw(volumeMeterRed);
		if (displaySelection != 0) {
			window.draw(fullscreen);
		}
		else {
			window.draw(windowed);
		}
		window.display();
	}
	return -1;
}


#pragma once
#include "Screen.h"
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include "Loader.h"

class MenuScreen : public Screen {
public:
	MenuScreen();
	~MenuScreen() {}
	virtual int run(sf::RenderWindow &window);

	//sf::Music music;
	sf::View view;
};
MenuScreen::MenuScreen() {
}

int MenuScreen::run(sf::RenderWindow &window) {
	sf::Event event;
	this->view.reset(sf::FloatRect(0, 0, 1920, 1080));
	Loader loader;
	bool isRunning = true;
	sf::Texture bgTex(loader.loadTexture("assets/pics/outer_space.jpg"));
	sf::Sprite bgSprite;
	bgSprite.setTexture(bgTex);
	bgSprite.setColor(sf::Color(255, 255, 255, 255));
	bgSprite.setPosition(0, 0);
	sf::Font font(loader.loadFont("assets/fonts/space_age.ttf"));
	sf::Text play("Play", font);
	sf::Text options("Options", font);
	sf::Text exit("Exit", font);
	int selection = 0;
	sf::Image logo;
	if (!logo.loadFromFile("assets/pics/logo.png")) {
		cout << "Could not open image: assets/pics/logo.png" << endl;
	}
	logo.loadFromFile("assets/pics/logo.png");
	logo.createMaskFromColor(sf::Color::Black);
	sf::Texture logoTex;
	logoTex.loadFromImage(logo);
	sf::Sprite logoSprite;
	logoSprite.setTexture(logoTex);
	logoSprite.setPosition(view.getSize().x / 2 - logoSprite.getLocalBounds().width / 2, 10);
	
	highscore = loader.loadHighscoreFromFile();

	sf::SoundBuffer clipBuffer;
	clipBuffer.loadFromFile("assets/sounds/clip.wav");
	sf::Sound clip;
	clip.setBuffer(clipBuffer);

	if (music.getStatus() != sf::Music::Playing) {
		if (!music.openFromFile("assets/sounds/Ossuary5_Rest.ogg")) {
			cout << "Could not open assets/sounds/Ossuary5_Rest.ogg" << endl;
		}
		music.openFromFile("assets/sounds/Ossuary5_Rest.ogg");
		music.setLoop(true);
		music.play();
	}

	play.setCharacterSize(60);
	play.setColor(sf::Color(255, 255, 255, 255));
	play.setPosition(view.getSize().x / 2 - play.getLocalBounds().width / 2, 500);

	options.setCharacterSize(60);
	options.setColor(sf::Color(255, 255, 255, 255));
	options.setPosition(view.getSize().x / 2 - options.getLocalBounds().width / 2, 600);

	exit.setCharacterSize(60);
	exit.setColor(sf::Color(255, 255, 255, 255));
	exit.setPosition(view.getSize().x / 2 - exit.getLocalBounds().width / 2, 700);

	while (isRunning) {
		music.setVolume(musicVolume);
		while (window.pollEvent(event))
			switch (event.type) {
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
					return -1;
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
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) || sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
					if (selection == 0) {
						return 5;
					}
					else if(selection == 1){
						return 3;
					}
					else {
						return -1;
					}
					break;
				}
			default:
				break;
			}
		if (selection < 0) {
			selection = 2;
		}
		else if(selection > 2) {
			selection = 0;
		}
		bgSprite.setColor(sf::Color(255, 255, 255, 255));
		logoSprite.setColor(sf::Color(255, 255, 255, 255));
		switch (selection) {
		case 0:
			play.setColor(sf::Color(255, 0, 0, 255));
			options.setColor(sf::Color(255, 255, 255, 255));
			exit.setColor(sf::Color(255, 255, 255, 255));
			break;
		case 1:
			play.setColor(sf::Color(255, 255, 255, 255));
			options.setColor(sf::Color(255, 0, 0, 255));
			exit.setColor(sf::Color(255, 255, 255, 255));
			break;
		case 2:
			play.setColor(sf::Color(255, 255, 255, 255));
			options.setColor(sf::Color(255, 255, 255, 255));
			exit.setColor(sf::Color(255, 0, 0, 255));
			break;
		}
		window.clear();
		window.setView(view);
		window.draw(bgSprite);
		window.draw(logoSprite);
		window.draw(play);
		window.draw(options);
		window.draw(exit);
		window.display();
	}
	return -1;
}
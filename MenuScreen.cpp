#include "MenuScreen.h"

MenuScreen::MenuScreen() {
	this->isPlaying = false;
}
MenuScreen::~MenuScreen() {}

int MenuScreen::run(sf::RenderWindow &window) {
	sf::Event event;
	Loader loader;
	bool isRunning = true;
	sf::Texture bgTex(loader.loadTexture("assets/pics/bg_space.jpg"));
	sf::Sprite bgSprite;
	bgSprite.setTexture(bgTex);
	sf::Font font(loader.loadFont("assets/fonts/tahoma.ttf"));
	sf::Text play("Play", font);
	sf::Text exit("Exit", font);
	int menu = 0;

	play.setCharacterSize(60);
	play.setPosition(800, 600);

	exit.setCharacterSize(60);
	exit.setPosition(800, 700);

	while (isRunning) {
		while (window.pollEvent(event))
			switch (event.type) {
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
					window.close();
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
					menu = 0;
					break;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
					menu = 1;
					break;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
					if (menu == 0) {
						isPlaying = true;
						return 1;
					}
					else {
						return -1;
					}
					break;
				}
			default:
				break;
			}
		if (menu == 0) {
			play.setColor(sf::Color(255, 0, 0, 255));
			exit.setColor(sf::Color(255, 255, 255, 255));
		}
		else {
			play.setColor(sf::Color(255, 255, 255, 255));
			exit.setColor(sf::Color(255, 0, 0, 255));
		}
		window.clear();
		window.draw(bgSprite);
		window.display();
	}
	
}

#pragma once
#include "Screen.h"
#include <SFML\Graphics.hpp>
#include "Loader.h"

class MissionsScreen : public Screen {
public:
	MissionsScreen();
	~MissionsScreen() {}
	virtual int run(sf::RenderWindow &window);
private:
	sf::View view;
	sf::Event event;
	Loader loader;
	sf::Sprite bgSprite;
	sf::Sprite mission1Spr;
	sf::Sprite mission2Spr;
	sf::RectangleShape missionBorder;
	sf::Font font;
};
MissionsScreen::MissionsScreen() {
}

int MissionsScreen::run(sf::RenderWindow &window) {
	
	this->view.reset(sf::FloatRect(0, 0, sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height));
	
	this->isRunning = true;
	font = loader.loadFont("assets/fonts/space_age.ttf");
	sf::Texture bgTex(loader.loadTexture("assets/pics/outer_space.jpg"));
	
	bgSprite.setTexture(bgTex);
	bgSprite.setPosition(0, 0);

	sf::Sound clip = loader.loadSound("assets/sounds/clip.wav");
	
	sf::Text missionSelect("Available missions", font);
	missionSelect.setCharacterSize(60);
	missionSelect.setPosition(view.getSize().x / 2 - missionSelect.getLocalBounds().width / 2, 20);

	sf::Text highscore;
	highscore.setFont(font);
	highscore.setCharacterSize(40);

	sf::Text mission1("Pacify the Alpha Quadrant", font);
	mission1.setCharacterSize(40);
	mission1.setPosition(view.getSize().x / 2 - mission1.getLocalBounds().width / 2, 300);
	sf::Text mission1Description("The enemy has occupied a critical\npart of Alpha Quadrant.\nWe need to drive them out before\nthey take the whole sector.", font);
	mission1Description.setCharacterSize(35);
	mission1Description.setPosition(view.getSize().x / 2 - mission1Description.getLocalBounds().width / 2, 700);
	sf::Texture mission1Tex(loader.loadTexture("assets/pics/bg_space.jpg"));
	
	mission1Spr.setTexture(mission1Tex);
	mission1Spr.setScale(0.2, 0.2);
	
	missionBorder.setSize(sf::Vector2f(392, 248));

	sf::Text mission2("Stop the resource gathering", font);
	mission2.setCharacterSize(40);
	mission2.setPosition(view.getSize().x / 2 - mission1.getLocalBounds().width / 2, 300);
	sf::Text mission2Description("The enemy is conducting a large\nmining operation on the surface of\nNX-237-A. We can't let them gather\nanymore resources.", font);
	mission2Description.setCharacterSize(35);
	mission2Description.setPosition(view.getSize().x / 2 - mission1Description.getLocalBounds().width / 2, 700);
	sf::Texture mission2Tex(loader.loadTexture("assets/pics/blue_space.jpg"));
	
	mission2Spr.setTexture(mission2Tex);
	mission2Spr.setScale(0.2, 0.2);

	sf::CircleShape back(40, 3);
	back.setFillColor(sf::Color::White);
	back.setRotation(-90);
	back.setPosition(view.getSize().x / 4, 550);

	sf::CircleShape forward(40, 3);	
	forward.setRotation(90);
	forward.setPosition(view.getSize().x / 4 + view.getSize().x / 2, 470);
	forward.setFillColor(sf::Color::White);

	int selection = 0;

	while (this->isRunning) {
		while (window.pollEvent(event))
			switch (event.type) {
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
					return 0;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) || sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
					this->isRunning = false;
					music.pause();
					if (selection == 0) {
						return 1;
					}
					else {
						return 2;
					}
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
					clip.play();
					selection += 1;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
					clip.play();
					selection -= 1;
				}
			}
		if (selection < 0) {
			selection = 0;
		}
		else if (selection > 1) {
			selection = 1;
		}
		window.clear();
		window.draw(bgSprite);
		window.draw(missionSelect);
		window.draw(back);	
		window.draw(forward);
		
		switch (selection) {
		case 0:
			mission1Spr.setPosition(view.getSize().x / 2 - mission1Spr.getTextureRect().width * 0.2 / 2, 400);
			missionBorder.setFillColor(sf::Color::Red);
			missionBorder.setPosition(mission1Spr.getPosition().x - 4, mission1Spr.getPosition().y - 4);
			highscore.setString("Highscore: " + to_string(highscores[0]));
			highscore.setPosition(view.getSize().x / 2 - highscore.getLocalBounds().width / 2, 900);
			window.draw(mission1);
			window.draw(missionBorder);
			window.draw(mission1Spr);
			window.draw(mission1Description);
			window.draw(highscore);
			break;
		case 1:
			mission2Spr.setPosition(view.getSize().x / 2 - mission2Spr.getTextureRect().width * 0.2 / 2, 400);
			missionBorder.setFillColor(sf::Color::Red);
			missionBorder.setPosition(mission2Spr.getPosition().x - 4, mission2Spr.getPosition().y - 4);
			highscore.setString("Highscore: " + to_string(highscores[1]));
			highscore.setPosition(view.getSize().x / 2 - highscore.getLocalBounds().width / 2, 900);
			window.draw(mission2);
			window.draw(missionBorder);
			window.draw(mission2Spr);
			window.draw(mission2Description);
			window.draw(highscore);
			break;
		}		
		window.display();
	}
	return -1;
}
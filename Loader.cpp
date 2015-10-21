#include "Loader.h"

Loader::Loader() {}
Loader::~Loader() {}


void Loader::loadMusic(string path) {
	if (!this->music.openFromFile(path)) {
		cout << "Could not open " << path << endl;
	}
	this->music.setVolume(50);
	this->music.setLoop(true);
	this->music.play();
}
sf::Font Loader::loadFont(string path) {
	sf::Font font;
	if (!font.loadFromFile(path)) {
		cout << "Could not load font: " << path << endl;
	}
	font.loadFromFile(path);
	return font;
}
sf::Texture Loader::loadTexture(string path) {
	sf::Texture texture;
	if(!texture.loadFromFile(path)) {
		cout << "Could not load texture: " << path << endl;
	}
	texture.loadFromFile(path);
	return texture;
}
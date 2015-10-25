#include "Loader.h"

Loader::Loader() {}
Loader::~Loader() {}

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
void Loader::saveHighscoreToFile(int score) {
	string filename = "score.bin";
	fstream stream(filename, ios_base::out | ios_base::binary);
	stream.close();
	stream.open(filename, ios_base::out | ios_base::binary);
	if (!stream.is_open()) {
		cout << "Can't open file " << filename << endl;
	}
	stream.write((char*)&score, sizeof(score));
	stream.close();
}
int Loader::loadHighscoreFromFile() {
	string filename = "score.bin";
	fstream stream;
	stream.open(filename, ios_base::in | ios_base::binary);
	int score;
	if (!stream.is_open()) {
		cout << "Can't open file " << filename << endl;
	}
	stream.read((char*)&score, sizeof(score));
	stream.close();
	return score;
}
#include "Loader.h"

Loader::Loader() {}
Loader::~Loader() {}

sf::Font Loader::loadFont(string path) {
	sf::Font font;
	try {
		font.loadFromFile(path);
	}
	catch (const exception &e) {
		cout << "Could not load font: " << path << " Exception: " << e.what() << endl;
	}
	return font;
}

sf::Texture Loader::loadTexture(string path) {
	sf::Texture texture;
	try {
		texture.loadFromFile(path);
	}
	catch (const exception &e) {
		cout << "Could not load texture: " << path << " Exception: " << e.what() << endl;
	}
	return texture;
}
void Loader::createSaveFile() {
	string filename = "score.bin";
	fstream stream;
	stream.exceptions(fstream::failbit | fstream::badbit);
	try {
		stream.open(filename, ios_base::out | ios_base::binary);
		cout << "New file created." << endl;
	}
	catch (fstream::failure e) {
		cout << "Could not open file " << filename << " Exception " << e.what() << endl;
	}
	try {
		stream.close();
		cout << "File " << filename << " closed succesfully." << endl;
	}
	catch (fstream::failure ex) {
		cout << "Could not close file " << filename << " Exception " << ex.what() << endl;
	}
}
void Loader::saveHighscoreToFile(int score) {
	string filename = "score.bin";
	fstream stream;
	stream.exceptions(fstream::failbit | fstream::badbit);
	try {
		stream.open(filename, ios_base::out | ios_base::binary);
		stream.write((char*)&score, sizeof(score));
		cout << "File " << filename << " opened succesfully." << endl;
	}
	catch (fstream::failure &e) {
		cout << "Could not open file " << filename << " Exception " << e.what() << endl;
		createSaveFile();
	}
	try {
		stream.close();
		cout << "File " << filename << " closed succesfully." << endl;
	}
	catch (fstream::failure &ex) {
		cout << "Could not close file " << filename << " Exception " << ex.what() << endl;
	}
}
void Loader::loadMusic(string path) {
	extern sf::Music music;
	music.openFromFile(path);
	music.setLoop(true);
}
sf::Image Loader::loadImage(string path) {
	sf::Image image;
	try {
		image.loadFromFile(path);
	}
	catch (const exception &e) {
		cout << "Could not load image: " << path << " Exception: " << e.what() << endl;
	}
	return image;
}
sf::Sound Loader::loadSound(string path)
{
	sf::Sound sound;
	sf::SoundBuffer *buffer = new sf::SoundBuffer();
	buffer->loadFromFile(path);
	sound.setBuffer(*buffer);
	return sound;
}
int Loader::loadHighscoreFromFile() {
	string filename = "score.bin";
	fstream stream;
	stream.exceptions(fstream::failbit | fstream::badbit);
	int score = 0;
	try {
		stream.open(filename, ios_base::in | ios_base::binary);
		stream.read((char*)&score, sizeof(score));
		cout << "File " << filename << " opened succesfully." << endl;
	}
	catch (fstream::failure &e) {
		cout << "Could not open file " << filename << " Exception " << e.what() << endl;
		createSaveFile();
	}
	try {
		stream.close();
		cout << "File " << filename << " closed succesfully." << endl;
	}
	catch (fstream::failure &ex) {
		cout << "Could not close file " << filename << " Exception " << ex.what() << endl;
	}
	return score;
}
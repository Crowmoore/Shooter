#include "Loader.h"

Loader::Loader() {}
Loader::~Loader() {}

//Try to load font from the given file path.
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
//Create a binary file. Apparently fstream does not throw exceptions by default so we have to say that we expect failbits and badbits.
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
//Save highscore to binary file. If a file cannot be opened or it doesn't exist, call the createSaveFile function.
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
//Try to load music from given file path.
void Loader::loadMusic(string path) {
	extern sf::Music music;
	try {
		music.openFromFile(path);
	}
	catch (const exception &e) {
		cout << "Could not load music: " << path << " Exception: " << e.what() << endl;
	}
	music.setLoop(true);
}
//Try to load an image and return it.
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
//Load a sound, set it to a new SoundBuffer and return the sound. 
sf::Sound Loader::loadSound(string path)
{
	sf::Sound sound;
	sf::SoundBuffer *buffer = new sf::SoundBuffer();
	try {
		buffer->loadFromFile(path);
	}
	catch (const exception &e) {
		cout << "Could not load sound: " << path << " Exception: " << e.what() << endl;
	}
	sound.setBuffer(*buffer);
	return sound;
}
//Try to read previous highscore from a binary file and return it.
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
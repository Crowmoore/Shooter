#pragma once
#include <SFML\Graphics.hpp>

class Explosion : public sf::Sprite {
public:
	Explosion(sf::Texture &);
	~Explosion();

	void explode(sf::RenderWindow &);
	bool getExploded();
	void setExploded(bool);
private:
	int frameCount;
	bool hasExploded;
};
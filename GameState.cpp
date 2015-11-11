#include "GameState.h"

//This is only used when pausing and unpausing the game.
GameState::GameState() {
	this->gameState = 0;
}
GameState::~GameState() {}

void GameState::setGameState(int state) {
	this->gameState = state;
}
int GameState::getGameState() {
	return this->gameState;
}
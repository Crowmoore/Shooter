#include "GameState.h"

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
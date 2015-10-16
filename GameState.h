#pragma once

class GameState {
public:
	GameState();
	~GameState();
	void setGameState(int);
	int getGameState();

	int gameState;
};
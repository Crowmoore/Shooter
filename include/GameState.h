#pragma once

class GameState {
public:
	GameState();
	~GameState();
	void setGameState(int);
	int getGameState();
private:
	int gameState;
};
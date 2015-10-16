#pragma once
#include "Enemy.h"
#include <vector>

using namespace std;

class Logics {
public:
	Logics();
	~Logics();
	
	void handleEnemyCollision(vector <Enemy>);
};
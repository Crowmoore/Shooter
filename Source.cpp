#include <SFML\Graphics.hpp>

#include "GameScreens.h"

using namespace std;

int main()
{
	vector <Screen *> screens;
	int currentScreen = 0;
	sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Project T.E.A.R");
	window.setMouseCursorVisible(false);

	MenuScreen menu;
	screens.push_back(&menu);
	Level1 level1;
	screens.push_back(&level1);
	PauseScreen pause;
	screens.push_back(&pause);
	OptionsScreen options;
	screens.push_back(&options);
	PointsScreen points;
	screens.push_back(&points);
	MissionsScreen missions;
	screens.push_back(&missions);

	while (currentScreen >= 0) {
		currentScreen = screens[currentScreen]->run(window);
	}
}
#include <SFML\Graphics.hpp>

#include "include/GameScreens.h"


using namespace std;

int main()
{
	vector <Screen *> screens;
	int currentScreen = 0;
	sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Project T.E.A.R");
	window.setMouseCursorVisible(false);

	//Create the screens needed for the game and push them to the screens vector.
	MenuScreen menu;
	screens.push_back(&menu);
	Level1 level1;
	screens.push_back(&level1);
	Level2 level2;
	screens.push_back(&level2);
	OptionsScreen options;
	screens.push_back(&options);
	PointsScreen points;
	screens.push_back(&points);
	MissionsScreen missions;
	screens.push_back(&missions);

	//Call the run method from a screen located at currentScreen index.
	while (currentScreen >= 0) {
		currentScreen = screens[currentScreen]->run(window);
	}
}
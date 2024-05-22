#pragma once
#include "SFML/Graphics.hpp"

#define MENU_ITEMS 3
#define OPTION_ITEMS 4
#define GAMEOVER_ITEMS 2
class Menu
{
public:
	// determines the current menu status
	enum class State {
		MainMenu,
		Options,
		GameOver
	};

	Menu(float width, float height);
	~Menu();

	// draws a menu on the screen
	void draw(sf::RenderWindow& window);

	// methods enabling navigation through the menu
	void MoveUp();
	void MoveDown();

	// event handling
	int handleEvent(const sf::Event& event);
	//returning the entered value from the menu
	int getMaxCars();

private:
	// stores the window size
	float x_size, y_size;
	// stores the value from the menu
	std::string maxCars;
	// stores the actual state of the menu
	State state;
	// stores the current index of the selected menu option
	int selectedItemIndex;
	// stores the font of the menu
	sf::Font font;
	// stores all of the items of menu state
	sf::Text menu[MENU_ITEMS];
	// stores all of the items of option state
	sf::Text options[OPTION_ITEMS];
	// stores the text "Game Over"
	sf::Text gameOverText;
	// stores all of the items of gameover state
	sf::Text gameOver[GAMEOVER_ITEMS];
	// event handling for main_menu state
	int handleMainMenuEvent(const sf::Event& event);
	// event handling for game_over state
	int handleGameOverEvent(const sf::Event& event);
	// event handling for options state
	bool handleOptionsEvent(const sf::Event& event);
	// updates position of elements in menu
	void updatePosition();
	// updates position of string maxCars in menu
	void updatePositionText();
	


};

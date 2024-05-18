#pragma once
#include "SFML/Graphics.hpp"

#define MENU_ITEMS 3
#define OPTION_ITEMS 4
#define GAMEOVER_ITEMS 2
class Menu
{
public:
	enum class State {
		MainMenu,
		Options,
		GameOver
	};

	Menu(float width, float height);
	~Menu();

	void draw(sf::RenderWindow& window);
	void MoveUp();
	void MoveDown();
	int GetPressedItem() { return selectedItemIndex; }
	int handleEvent(const sf::Event& event);
	int getMaxCars();

private:
	float x_size, y_size;
	std::string maxCars;
	State state;
	int selectedItemIndex;
	sf::Font font;
	sf::Text menu[MENU_ITEMS];
	sf::Text options[OPTION_ITEMS];
	sf::Text gameOverText;
	sf::Text gameOver[GAMEOVER_ITEMS];
	int handleMainMenuEvent(const sf::Event& event);
	int handleGameOverEvent(const sf::Event& event);
	bool handleOptionsEvent(const sf::Event& event);
	void updatePosition();
	void updatePositionText();


};

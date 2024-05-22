#include "Menu.h"
#include <cstdlib>
#include <iostream>
Menu::Menu(float width, float height)
{
	this->maxCars = "5";
	this->x_size = width;
	this->y_size = height;
	font.loadFromFile("arial.ttf");

	state = State::MainMenu;

	//main menu
	menu[0].setFont(font);
	menu[0].setFillColor(sf::Color::Red);
	menu[0].setString("Play");

	menu[1].setFont(font);
	menu[1].setFillColor(sf::Color::White);
	menu[1].setString("Options");

	menu[2].setFont(font);
	menu[2].setFillColor(sf::Color::White);
	menu[2].setString("Exit");

	//options
	options[0].setFont(font);
	options[0].setString("Maksymalna liczba pojazdow na mapie: ");
	options[0].setCharacterSize(24);
	options[0].setFillColor(sf::Color::White);



	options[1].setFont(font);
	options[1].setString(maxCars);
	options[1].setCharacterSize(24);
	options[1].setFillColor(sf::Color::White);


	options[2].setFont(font);
	options[2].setString("Save");
	options[2].setCharacterSize(24);
	options[2].setFillColor(sf::Color::Red);

	//gameover
	

	gameOverText.setFont(font);
	gameOverText.setString("Game Over!");
	gameOverText.setCharacterSize(24);
	gameOverText.setFillColor(sf::Color::White);



	gameOver[0].setFont(font);
	gameOver[0].setString("Play again!");
	gameOver[0].setCharacterSize(24);
	gameOver[0].setFillColor(sf::Color::Red);

	gameOver[1].setFont(font);
	gameOver[1].setString("Main menu");
	gameOver[1].setCharacterSize(24);
	gameOver[1].setFillColor(sf::Color::White);
	


	updatePosition();

	selectedItemIndex = 0;
}
int Menu::getMaxCars() {
	return stoi(maxCars);
}
int Menu::handleMainMenuEvent(const sf::Event& event) {
	switch (event.type) {
	case sf::Event::KeyPressed:
		if (event.key.code == sf::Keyboard::Up)
			this->MoveUp();
		if (event.key.code == sf::Keyboard::Down)
			this->MoveDown();

		if (event.key.code == sf::Keyboard::Enter) {
			if (selectedItemIndex == 1)
				state = State::Options;
			if (selectedItemIndex == 0) {
				state = State::GameOver;

			}
			return selectedItemIndex;
		}
		
	}
	return -1;

}
int Menu::handleEvent(const sf::Event& event) {
	switch (state) {
	case State::MainMenu:
		return handleMainMenuEvent(event);
	case State::Options:
		return handleOptionsEvent(event);
	case State::GameOver:
		return handleGameOverEvent(event);
	}
	return -2;
}
bool Menu::handleOptionsEvent(const sf::Event& event) {

	switch (event.type) {
	case sf::Event::KeyPressed:
		if (event.key.code == sf::Keyboard::Enter)
			state = State::MainMenu;
		break;
	case sf::Event::TextEntered:
		if (event.text.unicode >= 48 && event.text.unicode <= 57) {
			maxCars += static_cast<char>(event.text.unicode);
			options[1].setString(maxCars);

		}
		else if (event.text.unicode == 8 && !maxCars.empty()) { 
			maxCars.pop_back();
			options[1].setString(maxCars);
		}
		updatePositionText();
	}
	return true;

}
int Menu::handleGameOverEvent(const sf::Event& event) {
	switch (event.type) {
	case sf::Event::KeyPressed:
		if (event.key.code == sf::Keyboard::Up)
				this->MoveUp();
		if (event.key.code == sf::Keyboard::Down)
				this->MoveDown();

		if (event.key.code == sf::Keyboard::Enter) {
			if (selectedItemIndex == 1) {
				state = State::MainMenu;
				this->MoveUp();
				selectedItemIndex = 0;
				return -1;
			}
			return selectedItemIndex;
		}
		

	}
	return -1;
}
Menu::~Menu()
{
}
void Menu::updatePositionText() {
	sf::FloatRect bounds = options[1].getLocalBounds();
	options[1].setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
	options[1].setPosition(x_size / 2.0f, y_size / (OPTION_ITEMS + 1) * 2);
}
void Menu::updatePosition() {
	for (int i = 0; i < MENU_ITEMS; i++) {
		sf::FloatRect bounds = menu[i].getLocalBounds();
		menu[i].setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
		menu[i].setPosition(x_size / 2.0f, y_size / (MENU_ITEMS + 1) * (i + 1));
	}
	for (int i = 0; i < OPTION_ITEMS; i++) {
		sf::FloatRect bounds = options[i].getLocalBounds();
		options[i].setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
		options[i].setPosition(x_size / 2.0f, y_size / (OPTION_ITEMS + 1) * (i + 1));
	}
	
	sf::FloatRect bounds = gameOverText.getLocalBounds();
	gameOverText.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
	gameOverText.setPosition(x_size / 2.0f, y_size / (OPTION_ITEMS + 1) *  1);
	for (int i = 0; i < GAMEOVER_ITEMS; i++) {
		sf::FloatRect bounds = gameOver[i].getLocalBounds();
		gameOver[i].setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
		gameOver[i].setPosition(x_size / 2.0f, y_size / (OPTION_ITEMS + 1) * (i + 2));
	}

}


void Menu::draw(sf::RenderWindow& window)
{
	switch (state) {
	case State::MainMenu:
		for (int i = 0; i < MENU_ITEMS; i++)
		{
			window.draw(menu[i]);
		}
		break;
	case State::Options:
		for (int i = 0; i < OPTION_ITEMS; i++)
		{
			window.draw(options[i]);
		}
		break;
	case State::GameOver:
		window.draw(gameOverText);
		for (int i = 0; i < GAMEOVER_ITEMS; i++)
		{
			window.draw(gameOver[i]);
		}
		break;
	}
}

void Menu::MoveUp()
{
	switch (state) {
	case State::MainMenu:
		if (selectedItemIndex - 1 >= 0)
		{
			menu[selectedItemIndex].setFillColor(sf::Color::White);
			selectedItemIndex--;
			menu[selectedItemIndex].setFillColor(sf::Color::Red);
		}
		break;
	case State::GameOver:
		std::cout << "selectedItemIndex: " << selectedItemIndex << std::endl;
		if (selectedItemIndex - 1 >= 0) {
			gameOver[selectedItemIndex].setFillColor(sf::Color::White);
			selectedItemIndex--;
			gameOver[selectedItemIndex].setFillColor(sf::Color::Red);
		}
		break;
	}

}

void Menu::MoveDown()
{
	switch (state) {
	case State::MainMenu:
		if (selectedItemIndex + 1 < MENU_ITEMS)
		{
			menu[selectedItemIndex].setFillColor(sf::Color::White);
			selectedItemIndex++;
			menu[selectedItemIndex].setFillColor(sf::Color::Red);
		}
		break;
	case State::GameOver:
		if (selectedItemIndex +1 < GAMEOVER_ITEMS) {
			gameOver[selectedItemIndex].setFillColor(sf::Color::White);
			selectedItemIndex++;
			gameOver[selectedItemIndex].setFillColor(sf::Color::Red);
		}
		break;
	}
}
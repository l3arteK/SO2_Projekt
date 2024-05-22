#include <iostream>
#include <vector>
#include <ctime>
#include "Car.h"
#include "menu.h"
#include <thread>
#include <cstdlib>
#include <ctime>
#include <SFML/Graphics.hpp>

// stores the objects of the hearts
std::vector<sf::CircleShape> hearts;
// stores the thread of adding cars to the board
std::thread addCar_thread;
// stores the thread handling car collisions
std::thread collision_thread;
// a mutex enabling termination of the addCar_thread and restarting the game
std::mutex mutex_initGame; 
// stores the current number of cars on the screen
std::atomic<int>carsOnScreen = 0;
// a variable enabling termination of the addCar_thread and restarting the game
std::atomic<bool> finish = false;
// stores the max number of cars on the screen
int maxCars_ = 5;


// adds cars to the board
void addCar() {
	while (!finish) {
			if (Car::objects.size() < maxCars_) {
				mutex_initGame.lock();
				new Car();
				carsOnScreen++;
				mutex_initGame.unlock();
			}
		std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 1000 + 500 * (maxCars_ - carsOnScreen)));
		}
	}
// initializes the game
void initGame(){

	addCar_thread = std::thread(addCar);
	collision_thread = std::thread(Car::checkAllCollisions);

}
// resets the game
void resetGame(bool r = true) {
	mutex_initGame.lock();

	Car::checkingCollision = false;
	finish = true;

	Car::objects.clear();

	if (collision_thread.joinable()) 
		collision_thread.join();

	if (addCar_thread.joinable()) 
		addCar_thread.join();

	Car::checkingCollision = true;
	carsOnScreen = 0;
	finish = false;

	if(r)
		initGame();

	mutex_initGame.unlock();
	
}
// draws the health
void drawHealth(sf::RenderWindow& window) {
	for (auto& obj:hearts) {
		window.draw(obj);
	}
}
// generate health objects
void generateHealt(int health) {
	for (int i = 0; i < health; i++) {
		sf::CircleShape ch;
		ch.setFillColor(sf::Color::Red);
		ch.setRadius(8);
		float x = SCREEN_WIDTH - 30 * (i + 1);
		ch.setPosition({ x, 25 });
		hearts.push_back(ch);
	}
}
// main function
int main() {
	bool showMenu = true;
	Menu menu(SCREEN_WIDTH,SCREEN_HEIGHT);

	srand(time(NULL));
	// create window
	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Symulator_ruchu_drogowe", sf::Style::Titlebar | sf::Style::Close);
	sf::Event ev;
	window.setFramerateLimit(60);

	// horizontal road
	sf::RectangleShape horizontal(sf::Vector2f(800, 200));
	horizontal.setFillColor(sf::Color(128, 128, 128));
	horizontal.setPosition(0, 200);

	//vertical road
	sf::RectangleShape vertical(sf::Vector2f(200, 600));
	vertical.setFillColor(sf::Color(128, 128, 128));
	vertical.setPosition(300, 0);

	// game loop
	while (window.isOpen()) {
		// check if collision
		if (Car::collision) {
			if (hearts.size() > 1) {
				hearts.pop_back();
				std::cout << "hearts_size: " << hearts.size() <<std::endl;
			}
			else {
				resetGame(false);
				showMenu = true;
			}
			Car::collision = false;
		}
		while (window.pollEvent(ev)) {
			// handling events from the menu
			if (showMenu)
			{
				int option = menu.handleEvent(ev);
				if (option == 0) {
					maxCars_ = menu.getMaxCars();
					initGame();
					generateHealt(5);
					showMenu = false;

				}
				if (option == 2) {
					window.close();
				}
				if (ev.type == sf::Event::Closed)
					window.close();
			}
			else
			// handling events from the game
			switch (ev.type)
			{
				 
			case sf::Event::Closed:
				resetGame(false);
				window.close();
				break;
			case sf::Event::KeyPressed:
				if (ev.key.code == sf::Keyboard::Escape) {
					resetGame(false);
					showMenu = true;
				}
				if (ev.key.code == sf::Keyboard::A)
					resetGame();
				break;
			case sf::Event::MouseMoved:
				//checking if the mouse is not over the car
				for (Car* car : Car::objects) {
					if ((car->getPosition().x <= sf::Mouse::getPosition(window).x) && (car->getPosition().x + car->getSize().x >= sf::Mouse::getPosition(window).x) &&
						(car->getPosition().y <= sf::Mouse::getPosition(window).y) && (car->getPosition().y + car->getSize().y >= sf::Mouse::getPosition(window).y)) {
						car->stopCar();
					}
					else
						car->startCar();
				}
				break;
				
				
			}

		}

		window.clear(); 
		// draws menu or a game
		if (showMenu)
			menu.draw(window);
		else {
			window.draw(horizontal);
			window.draw(vertical);
			drawHealth(window);
			Car::draw(window);
		}
		
		window.display();
	

	}



	return 0;
}

#include <iostream>
#include <vector>
#include <ctime>
#include "Car.h"
#include "menu.h"
#include <thread>
#include <cstdlib>
#include <ctime>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
int max_height = 600, max_width = 800;
std::vector<std::thread> carThreads;
std::vector<sf::CircleShape> hearts;
std::thread addCar_thread;
std::thread collision_thread;
std::mutex mtx_initGame; 
std::condition_variable cv_initGame;
std::atomic<int>carsOnScreen = 0;
std::atomic<bool> finish = false;
int maxCars_ = 5;

void addCar() {
	while (!finish) {
			if (Car::objects.size() < maxCars_) {
				mtx_initGame.lock();
				new Car();
				carsOnScreen++;
				mtx_initGame.unlock();
			}
			if(!finish)
				std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 700 + 100 * (maxCars_ - carsOnScreen)));
			std::cout << std::this_thread::get_id() << std::endl;
		}
	}
void initGame(){

	addCar_thread = std::thread(addCar);
	collision_thread = std::thread(Car::checkAllCollisions);

}
void resetGame(bool r = true) {
	// ¿eby addCar nie dodawa³ ¿adnych nowych aut na plansze podczas resetowania
	mtx_initGame.lock();
	
	//aby mozna bylo zakonczyc watek zajmujacy sie kolizja
	Car::checkingCollision = false;
	//aby mozna bylo zakonczyc watek dodajacy samochody
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
	mtx_initGame.unlock();
	
}
void drawHealth(sf::RenderWindow& window) {
	for (auto& obj:hearts) {
		window.draw(obj);
	}
}
void generateHealt(int health) {
	for (int i = 0; i < health; i++) {
		sf::CircleShape ch;
		ch.setFillColor(sf::Color::Red);
		ch.setRadius(8);
		float x = max_width - 30 * (i + 1);
		ch.setPosition({ x, 25 });
		hearts.push_back(ch);
	}
}
int main() {
	bool showMenu = true;
	Menu menu(max_width,max_height);

	srand(time(NULL));
	sf::RenderWindow window(sf::VideoMode(max_width, max_height), "Symulator_ruchu_drogowe", sf::Style::Titlebar | sf::Style::Close);
	sf::Event ev;




	// Tworzenie obiektów kszta³tu
	sf::RectangleShape horizontal(sf::Vector2f(800, 200));
	sf::RectangleShape vertical(sf::Vector2f(200, 600));
	
	// Ustawienie koloru i pozycji dla poziomej linii krzy¿a
	horizontal.setFillColor(sf::Color(128,128,128));
	horizontal.setPosition(0, 200);

	// Ustawienie koloru i pozycji dla pionowej linii krzy¿a
	vertical.setFillColor(sf::Color(128, 128, 128));
	vertical.setPosition(300, 0);





	while (window.isOpen()) {
		if (Car::collision) {
			if (hearts.size() > 0)
				hearts.pop_back();
			else {
				resetGame(false);
				showMenu = true;
			}
			Car::collision = false;
		}
		while (window.pollEvent(ev)) {
			if (showMenu)
			{
				int option = menu.handleEvent(ev);
				//std::cout << "Option: " << option << std::endl;
				if (option == 0) {
					maxCars_ = menu.getMaxCars();
					initGame();
					generateHealt(3);
					showMenu = false;

				}
				if (option == 2) {
					window.close();
				}
				if (ev.type == sf::Event::Closed)
					window.close();
			}
			else
			switch (ev.type)
			{
				 
			case sf::Event::Closed:
				resetGame(false);
				window.close();
				break;
			case sf::Event::KeyPressed:
				if (ev.key.code == sf::Keyboard::Escape) {
					resetGame(false);
					window.close();
				}
				if (ev.key.code == sf::Keyboard::A)
					resetGame();
				break;
			case sf::Event::MouseMoved:
				/*std::cout << sf::Mouse::getPosition(window).x << " " << sf::Mouse::getPosition(window).y << std::endl;*/
				for (Car* car : Car::objects) {
					if ((car->getPosition().x <= sf::Mouse::getPosition(window).x) && (car->getPosition().x + car->getSize().x >= sf::Mouse::getPosition(window).x) &&
						(car->getPosition().y <= sf::Mouse::getPosition(window).y) && (car->getPosition().y + car->getSize().y >= sf::Mouse::getPosition(window).y)) {
						/*std::cout << "W kwadracie!" << std::endl;*/
						car->stopCar();
					}
					else
						car->startCar();
				}
				break;
				
				
			}

		}

		window.clear(); 
		if (showMenu)
			menu.draw(window);
		else {
			window.draw(horizontal);
			window.draw(vertical);
			drawHealth(window);
			for (auto& obj : Car::objects) {
				window.draw(*obj);
			}
		}
		
		window.display();
	

	}



	return 0;
}

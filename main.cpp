#include <iostream>
#include <vector>
#include <ctime>
#include "Car.h"
#include <thread>
#include <cstdlib>
#include <ctime>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
std::vector<std::thread> carThreads;
std::thread addCar_thread;
std::thread collision_thread;
std::mutex mtx_initGame;
std::condition_variable cv_initGame;
std::atomic<int>addedCar = 0;
std::atomic<bool> finish = false;
void addCar(int max) {
	
	while (addedCar < max && !finish) {
			mtx_initGame.lock();
			new Car(rand() % 4 + 1);
			carThreads.push_back(Car::objects.at(addedCar)->moveThread());
			if(!finish)
				std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 700 + 100 * (max - addedCar)));
			addedCar++;
			mtx_initGame.unlock();
			std::cout << std::this_thread::get_id() << std::endl;
		}
	std::cout <<"terminate_addCar: " << std::this_thread::get_id() << std::endl;
	}
void initGame(){

	addCar_thread = std::thread(addCar, 20);
	collision_thread = std::thread(Car::checkAllCollisions);

}
void resetGame() {
	mtx_initGame.lock();
	
	
	
	Car::checkingCollision = false;
	finish = true;
	for (auto& obj : Car::objects) {
		obj->moving = false;
	}
	for (auto &obj : carThreads) {
		if (obj.joinable()) {
			obj.join();
			std::cout << "join carMove_threaeed" << std::endl;
		}
	}
	Car::objects.clear();
	carThreads.clear();
	

	if (collision_thread.joinable()) {
		collision_thread.join();
		std::cout << "join collision_threaeed" << std::endl;
	}
	if (addCar_thread.joinable()) {
		addCar_thread.join();
		std::cout << "join addCar_threaeed" << std::endl;
	}
	Car::checkingCollision = true;
	addedCar = 0;
	finish = false;

	initGame();
	mtx_initGame.unlock();
	
}

int main() {

	srand(time(NULL));
	sf::RenderWindow window(sf::VideoMode(800, 600), "Symulator_ruchu_drogowe", sf::Style::Titlebar | sf::Style::Close);
	sf::Event ev;

	//Cars.push_back(new Car(1));
	//Cars.push_back(new Car(2));
	/*góra 340 5 425 5
		//lewo 10 240 10 325
		prawo 760 240 760 325
		do³ 340 560 425 560
		*/ 

	initGame();




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
		while (window.pollEvent(ev)) {
			switch (ev.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				if (ev.key.code == sf::Keyboard::Escape)
					window.close();
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
				//	/*std:: cout << car.getXpos() << " " << car.getYpos() << std::endl;*/
				}
				break;
				
			}

		}

		//Update
		window.clear(); //clear old frame

		window.draw(horizontal);
		window.draw(vertical);

		//draw your game

		
		for (auto &obj : Car::objects) {
			window.draw(*obj);
		}

		window.display();
	



		 //tell app that winndow is done drawing

		//Render
	}



	return 0;
}

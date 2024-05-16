#include <iostream>
#include <vector>
#include <ctime>
#include "Car.h"
#include <thread>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

int main() {

	sf::RenderWindow window(sf::VideoMode(800, 600), "Symulator_ruchu_drogowe", sf::Style::Titlebar | sf::Style::Close);
	sf::Event ev;
	std::vector<std::thread> carThreads;
	std::vector<Car*> Cars;
	Cars.push_back(new Car(330, 5));
	Cars.push_back(new Car(330, 15));


	carThreads.push_back(Cars[0]->moveThread(true));
	carThreads.push_back(Cars[1]->moveThread(false));
	std::thread collision_thread(Car::checkAllCollisions);

	carThreads.at(0).detach();
	carThreads.at(1).detach();
	collision_thread.detach();




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
					std::cout << "A" << std::endl;
				break;
			case sf::Event::MouseMoved:
				//std::cout << sf::Mouse::getPosition(window).x << " " << sf::Mouse::getPosition(window).y << std::endl;
				for (Car* car : Cars) {
					if ((car->getPosition().x <= sf::Mouse::getPosition(window).x) && (car->getPosition().x + car->getSize().x >= sf::Mouse::getPosition(window).x) &&
						(car->getPosition().y <= sf::Mouse::getPosition(window).y) && (car->getPosition().y + car->getSize().y >= sf::Mouse::getPosition(window).y)) {
						std::cout << "W kwadracie!" << std::endl;
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

		
		window.draw(*Cars[0]);
		window.draw(*Cars[1]);

		window.display();
	



		 //tell app that winndow is done drawing

		//Render
	}



	return 0;
}
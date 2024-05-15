#include "Car.h"
#include <iostream>
Car::Car(int xPosPar, int yPosPar) {
	this->xPos = xPosPar;
	this->yPos = yPosPar;
	this->setPosition(xPos, yPos);
	this->setSize(sf::Vector2f(25, 25));
	this->setFillColor(sf::Color::White);

}
int Car::getXpos() {
	return this->xPos;
}
int Car::getYpos() {
	return this->yPos;
}
void Car::UnicMove(bool p) {
	while (true) {
		if(p)
		this->move(0, 1);
		else
		this->move(1, 0);
		std::this_thread::sleep_for(std::chrono::microseconds(200));
	}
		
}
std::thread Car::moveThread(bool p) {
	return std::thread(&Car::UnicMove, this, p);

}
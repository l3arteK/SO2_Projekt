#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <thread>
class Car :public sf::RectangleShape{
public:
	Car(int xPos, int yPos);
	int getXpos();
	int getYpos();
	std::thread moveThread(bool p);
private:
	int xPos;
	int yPos;
	void UnicMove(bool p);
	


};
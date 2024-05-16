#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <thread>
#include <mutex>
#include <condition_variable>
class Car :public sf::RectangleShape{
public:
	Car(int xPos, int yPos);
	static std::vector<Car*> objects;
	static std::mutex mutex;
	static void checkAllCollisions();
	std::thread moveThread(bool p);
	bool checkCollison(const Car& other);
	std::pair<int, int> getPos() const;
	void stopCar();
	void startCar();
private:
	int xPos;
	int yPos;
	void UnicMove(bool p);
	std::mutex mutex_stop;
	bool stop;
	std::condition_variable cv_stop;
	bool moving;


	


};
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
	~Car();
	Car();
	static std::vector<Car*> objects;
	static std::mutex mutex;
	static void checkAllCollisions();
	std::thread moveThread();
	bool checkCollison(Car& other);
	std::pair<float, float> getPos() ;
	void stopCar();
	void startCar();
	std::atomic<bool> moving = true;
	static std::atomic<bool> checkingCollision;
	void setScreenSize(int width_screen, int height_screen);
	static bool collision;
private:
	void UnicMove();
	std::mutex mutex_stop;
	std::mutex mtx_getPos;
	bool stop;
	std::condition_variable cv_stop;
	int start_pos;
	float speed;
	static int width_screen, height_screen;
	void setStats();
	
	


	


};
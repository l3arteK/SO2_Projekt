#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <thread>
#include <mutex>
#include <condition_variable>
#define SCREEN_HEIGHT 600
#define SCREEN_WIDTH 800
class Car :public sf::RectangleShape{
public:
	~Car();
	Car();

	// draws a cars on the screen
	static void draw(sf::RenderWindow& window);
	// checks the collision between all cars
	static void checkAllCollisions();
	// stores all created cars
	static std::vector<Car*> objects;
	// protects access to 'objects'
	static std::mutex mutex_objects;
	// stores collision information
	static bool collision;
	// allows to stop the thread responsible for collision checking
	static std::atomic<bool> checkingCollision;

	// checks the collision between two cars
	bool checkCollison(Car& other);
	// stops the car
	void stopCar();
	// starts the car
	void startCar();
	
	// return actual position of car
	std::pair<float, float> getPos();
	// allows to stop the thread responsible for moving the car
	std::atomic<bool> moving = true;
private:
	// stores the start position of cars
	static int startPos[4][2];
	int start_pos;
	// moves the car
	void UnicMove();
	// sets the initial values of the object
	void setStats();

	// a flag indicating that the turn signal is showing or not
	std::atomic<bool> blink;
	// shape of blinker
	sf::CircleShape blinker;
	// allows to stop the car
	std::mutex mutex_stop;
	std::condition_variable cv_stop;\
	bool stop;
	//stores the unicMove function in thread
	std::thread mvThread;

	
	//stores the speed of the car
	float speed;

	// a flag indicating that the car may turn // 1 or 2 -> yes 
	int turn;

	// variables that help in drawing a turn signal
	int blinking;
	int to_blink;
	
	
	


	


};
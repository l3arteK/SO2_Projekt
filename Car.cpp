#include "Car.h"
#include <iostream>
Car::Car() {	
	
	setStats();
	this->stop = false;
	std::unique_lock<std::mutex> lock(Car::mutex);
	objects.push_back(this);
	this->mvThread = std::thread(&Car::UnicMove, this);
}
std::pair<float, float> Car::getPos()  {
	std::lock_guard<std::mutex> lock(mtx_getPos);
	sf::Vector2f vec = this->getPosition();
	return { vec.x, vec.y };
}
void Car::stopCar() {
	if(!this->stop){
		std::unique_lock<std::mutex> lock(mutex_stop);
		this->stop = true;
		cv_stop.notify_all();
	}

}
void Car::startCar() {
	if (this->stop) {
		std::unique_lock<std::mutex> lock(mutex_stop);
		this->stop = false;
		cv_stop.notify_all();
	}

}
void Car::setScreenSize(int width_screen, int height_screen)
{
	this->width_screen = width_screen;
	this->height_screen = height_screen;
}
void Car::UnicMove() {
	while (moving) {
		std::unique_lock<std::mutex> lock(mutex_stop);
		cv_stop.wait(lock, [this] { return !stop; });
		lock.unlock();
		std::pair<int, int> pos = this->getPos();
		if ((pos.first < width_screen+60 && pos.first > -60) && (pos.second < height_screen+60 && pos.second > -60)) {
			if (this->start_pos == 1) {
				if (turn != 0) {
					if (pos.second < (height_screen / 2) + this->getSize().y) {
						move(0, speed);
						blinker.move(0, speed);
					}
					else if (turn == 1)
						move(speed, 0);
					else if (turn == 2)
						move(-speed, 0);
				}else
					move(0, speed);
			}
			else if (this->start_pos == 2) {
				if (turn != 0) {
					if (pos.first < (width_screen / 2) + this->getSize().x)
						move(speed, 0);
					else if (turn == 1)
						move(0, speed);
					else if (turn == 2)
						move(0,-speed);
				}
				else
					move(speed, 0);
			}
			else if (this->start_pos == 3){
				if (turn != 0) {
					if (pos.first > (width_screen / 2) + this->getSize().x)
						move(-speed, 0);
					else if (turn == 1)
						move(0, speed);
					else if (turn == 2)
						move(0, -speed);
				}
				else
					move(-speed, 0);

				}
			else if (this->start_pos == 4)
			{
				if (turn != 0) {
					if (pos.first > (height_screen / 2) + this->getSize().x)
						move(0, -speed);
					else if (turn == 1)
						move(speed, 0);
					else if (turn == 2)
						move(-speed, 0);
				}
				else
					move(0, -speed);

			}	
		std::this_thread::sleep_for(std::chrono::microseconds(700));
		}
		else {
			this->setStats();
		}
		/*std::cout << "moving: " << std::this_thread::get_id() << std::endl;*/
	} 
	//std::cout << "terminate_moving: " << std::this_thread::get_id() << std::endl;	
}
std::thread Car::moveThread() {
	return std::thread(&Car::UnicMove, this);

}
void Car::checkAllCollisions() {
	std::cout << "start_checkAllCollisions: " << std::this_thread::get_id() << std::endl;
	while (checkingCollision) {
		std::unique_lock<std::mutex> lock(Car::mutex);
		for (int i = 0; i < objects.size(); i++) {
			for (int j = i + 1; j < objects.size(); j++) {
				if (objects[i]->checkCollison(*objects[j])) {
					collision = true;
					std::cout << "Kolizja!" << std::endl;
					Car* c1 = objects[i];
					Car* c2 = objects[j];
					lock.unlock();
					//c1->~Car();
					//c2->~Car();
					break;
				}
			}
			if (collision) {
				collision = false;
				break;
			}
		}
	
	}
	std::cout << "terminate_checkAllCollisions: " << std::this_thread::get_id() << std::endl;
}
void Car::setStats() {
	this->turn = 1;
	this->to_blink = 0;
	this->start_pos = 1;
	this->speed = (rand()%5 )/ 10.0f ;
	this->setSize(sf::Vector2f(25, 25));
	this->setOutlineColor(sf::Color::Black);
	this->setOutlineThickness(1);
	this->setFillColor(sf::Color::White);
	this->blinking = 0;



	if (start_pos == 1)
		this->setPosition(340, -50);
	else if (start_pos == 2)
		this->setPosition(-50, 240);
	else if (start_pos == 3)
		this->setPosition(this->width_screen+50, 325);
	else if (start_pos == 4)
		this->setPosition(425, this->height_screen+50);

	if (turn != 0)
	{
		blinker.setFillColor(sf::Color::Yellow);
		blinker.setRadius(4);
		blinker.setOutlineColor(sf::Color::Black);
		blinker.setOutlineThickness(1);
		if (start_pos == 1)
			if (turn == 1)
				blinker.setPosition(340 + 21, -50 + 21);

	}
	
}
 bool Car::checkCollison( Car& other) {
	 std::pair<float, float> othPos = other.getPos();
	 std::pair<float, float> thisPos = this->getPos();
	 int size1_x = this->getSize().x;
	 float size1_y = this->getSize().y;
	 float size2_x = other.getSize().x;
	 float size2_y = other.getSize().y;

	 float x1_1 = thisPos.first;
	 float x1_2 = thisPos.first + size1_x;
	 float y1_1 = thisPos.second;
	 float y1_2 = thisPos.second + size1_y;

	 float x2_1 = othPos.first;
	 float x2_2 = othPos.first + size2_x;
	 float y2_1 = othPos.second;
	 float y2_2 = othPos.second + size2_y;
	 //std::cout << othPos.first << " " << othPos.second << std::endl;
	 //std::cout << thisPos.first << " " << thisPos.second << std::endl;
	 return (!(x1_2 < x2_1 || x1_1 > x2_2 || y1_2 < y2_1 || y1_1 >  y2_2));
 }
 Car::~Car() { 
	 std::unique_lock<std::mutex> lock1(mutex_stop);
	 std::cout << "destructor Car!" << std::endl;
	 this->moving = false;
	 lock1.unlock();
	 this->startCar();
	 if (mvThread.joinable()) {
		 mvThread.join();
		 std::cout << "mvThread join" << std::endl;
	 }
		

	 std::unique_lock<std::mutex> lock(Car::mutex);
	 auto it = std::find(Car::objects.begin(), Car::objects.end(), this);
	 if (it != Car::objects.end()) {
		 std::cout << Car::objects.size() << std::endl;
		 Car::objects.erase(it);
		 std::cout << Car::objects.size() << std::endl;
	 }
 }
 void Car::draw(sf::RenderWindow &window) {
	 //std::unique_lock<std::mutex> lock(Car::mutex);
	
	 for (Car* obj : objects) {
		 window.draw(*obj);
		 if(obj->blinking > 40){
			 obj->to_blink =20 ;
			 obj->blinking = 0;
		 }
			
		 if (obj->to_blink > 0) {
			 obj->to_blink--;
			 window.draw(obj->blinker);
		 }
		 else
			 obj->blinking += 1;
	 }
 }

 std::mutex Car::mutex;
 std::vector<Car*> Car::objects;
 std::atomic<bool> Car::checkingCollision = true;
 int Car::height_screen = 600;
 int Car::width_screen = 800;
 bool Car::collision = false;
#include "Car.h"
#include <iostream>
#include <set>

std::mutex Car::mutex_objects;
std::vector<Car*> Car::objects;
std::atomic<bool> Car::checkingCollision = true;

bool Car::collision = false;
int Car::startPos[4][2] = {
	{340,-50},
	{-50, 325},
	{SCREEN_WIDTH + 50, 240},
	{425, SCREEN_HEIGHT + 50}
};
Car::Car() {	
	
	setStats();
	this->stop = false;
	std::unique_lock<std::mutex> lock(Car::mutex_objects);
	objects.push_back(this);
	this->mvThread = std::thread(&Car::UnicMove, this);
}
std::pair<float, float> Car::getPos()  {
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
void Car::UnicMove() {
	while (moving) {
		std::unique_lock<std::mutex> lock(mutex_stop);
		cv_stop.wait(lock, [this] { return !stop; });
		lock.unlock();
		std::pair<int, int> pos = this->getPos();

		if ((pos.first < SCREEN_WIDTH+60 && pos.first > -60) && (pos.second < SCREEN_HEIGHT +60 && pos.second > -60)) {
			if (this->start_pos == 1) {
				if (turn == 1 || turn == 2) {
					if ( (pos.second < (SCREEN_HEIGHT / 2) + this->getSize().y) && (pos.second < (SCREEN_HEIGHT / 2) + this->getSize().y -75 || turn!=2 )){
						move(0, speed);
						blinker.move(0, speed);
					}
					else if (turn == 1) {
						if (this->blink)
							this->blink = false;
						move(speed, 0);
						blinker.move(speed, 0);
					}
					else if (turn == 2) {
						if (this->blink)
							this->blink = false;
						move(-speed, 0);
						blinker.move(-speed, 0);
					}
				}else
					move(0, speed);
			}
			else if (this->start_pos == 2) {
				if (turn == 1 || turn == 2) {
					if ((pos.first < (SCREEN_WIDTH/ 2) + this->getSize().x) && (pos.first < (SCREEN_WIDTH / 2) + this->getSize().x - 75 || turn != 1) ) {
						move(speed, 0);
						blinker.move(speed, 0);
					}
					else if (turn == 1) {
						if (this->blink)
							this->blink = false;
						move(0, speed);
						blinker.move(0, speed);
					}
					else if (turn == 2) {
						if (this->blink)
							this->blink = false;
						move(0, -speed);
						blinker.move(0, -speed);
					}
				}
				else
					move(speed, 0);
			}
			else if (this->start_pos == 3){
				if (turn == 1 || turn == 2) {
					if ((pos.first > (SCREEN_WIDTH / 2) + this->getSize().x) || (pos.first > (SCREEN_WIDTH / 2) + this->getSize().x - 75 && turn == 1)) {
						move(-speed, 0);
						blinker.move(-speed, 0);
					}
					else if (turn == 1) {
						if (this->blink)
							this->blink = false;
						move(0, speed);
						blinker.move(0, speed);
					}
					else if (turn == 2) {
						if (this->blink)
							this->blink = false;
						move(0, -speed);
						blinker.move(0, -speed);
					}
				}
				else
					move(-speed, 0);

				}
			else if (this->start_pos == 4)
			{
				if (turn == 1 || turn == 2) {
					if ((pos.second > (SCREEN_HEIGHT / 2) + this->getSize().y) || ((pos.second > (SCREEN_HEIGHT / 2) + this->getSize().y - 75) && turn == 2)) {
						move(0, -speed);
						blinker.move(0, -speed);
					}
					else if (turn == 1 ) {
						if (this->blink)
							this->blink = false;
						move(speed, 0);
						blinker.move(speed, 0);
					}
					else if (turn == 2) {
						if (this->blink)
							this->blink = false;
						move(-speed, 0);
						blinker.move(-speed, 0);
					}
				}
				else
					move(0, -speed);

				
			}	
			
		std::this_thread::sleep_for(std::chrono::microseconds(1000));
		}
		else {
			this->setStats();
		}
	} 	
}

void Car::checkAllCollisions() {
	std::set <Car*> to_delete;
	while (checkingCollision) {
		std::unique_lock<std::mutex> lock(Car::mutex_objects);
		for (int i = 0; i < objects.size(); i++) {
			for (int j = i + 1; j < objects.size(); j++) {
				if (objects[i]->checkCollison(*objects[j])) {
					collision = true;
					to_delete.insert(objects[i]);
					to_delete.insert(objects[j]);
				}
			}

		}
		lock.unlock();
		for (auto& obj : to_delete)
			obj->~Car();
		to_delete.clear();
	}

}
void Car::setStats() {
	this->turn = rand()%5;
	blink = false;
	this->to_blink = 0;
	this->start_pos = rand()%4 + 1;

	this->speed = (rand()%3 )/ 10.0f + 0.1 ;
	this->setSize(sf::Vector2f(25, 25));
	this->setOutlineColor(sf::Color::Black);
	this->setOutlineThickness(1);
	this->setFillColor(sf::Color::White);
	this->blinking = 0;

	this->setPosition(startPos[start_pos-1][0], startPos[start_pos-1][1]);

	if (turn  == 1 || turn == 2)
	{
		blink = true;
		blinker.setFillColor(sf::Color::Yellow);
		blinker.setRadius(4);
		blinker.setOutlineColor(sf::Color::Black);
		blinker.setOutlineThickness(1);
		if (start_pos == 1)
			if (turn == 1)
				blinker.setPosition(startPos[0][0] + this->getSize().x - blinker.getRadius(), startPos[0][1] + this->getSize().x - blinker.getRadius());
			else
				blinker.setPosition(startPos[0][0] - (blinker.getRadius()/2), startPos[0][1] + this->getSize().x - blinker.getRadius());
		else if (start_pos == 2) {
			if(turn == 1)
				blinker.setPosition(startPos[1][0] + this->getSize().x - blinker.getRadius(), startPos[1][1] + this->getSize().x - blinker.getRadius());
			else
				blinker.setPosition(startPos[1][0] + this->getSize().x - blinker.getRadius(), startPos[1][1]);
		}
		else if (start_pos == 3) {
			if (turn == 1)
				blinker.setPosition(startPos[2][0] - (blinker.getRadius() / 2), startPos[2][1] + this->getSize().y - blinker.getRadius());
			else
				blinker.setPosition(startPos[2][0] - (blinker.getRadius() / 2) , startPos[2][1] - (blinker.getRadius() / 2));
		}
		else if (start_pos == 4) {
			if (turn == 1)
				blinker.setPosition(startPos[3][0] + this->getSize().x - blinker.getRadius(), startPos[3][1]);
				
			else
				blinker.setPosition(startPos[3][0] - (blinker.getRadius() / 2), startPos[3][1] - (blinker.getRadius() / 2));
		}



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
	 return (!(x1_2 < x2_1 || x1_1 > x2_2 || y1_2 < y2_1 || y1_1 >  y2_2));
 }
 Car::~Car() { 
	 std::unique_lock<std::mutex> lock1(mutex_stop);
	 this->moving = false;
	 lock1.unlock();
	 this->startCar();
	 if (mvThread.joinable()) 
		 mvThread.join();

	 std::unique_lock<std::mutex> lock(Car::mutex_objects);
	 auto it = std::find(Car::objects.begin(), Car::objects.end(), this);
	 if (it != Car::objects.end()) 
		 Car::objects.erase(it);

	 
 }
 void Car::draw(sf::RenderWindow &window) {
	 for (Car* obj : objects) {
		 window.draw(*obj);

		 if(obj->blinking > 40 && obj->blink ){
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




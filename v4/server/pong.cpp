#include "pong.h"
#define _USE_MATH_DEFINES
#define N 999 
#include <math.h>
#include <sstream>
#include <stdlib.h>
#include <iostream>


Pong::Pong(unsigned int width, unsigned int height) {
	this->width = width;
	this->height = height;
	init();
}

Pong::~Pong(){}
void Pong::init() {
	p.x = p.width;
	p.y = (height - p.height) / 2;
	p2.x = width - p2.width * 2;
	p2.y= (height - p.height) / 2;
	p3.x = (width - p3.width) / 2;
	p3.y = p3.height;
	p4.x = (width - p4.width) / 2;
	p4.y = height - p3.height * 2;

	b.x = width / 2;
	b.y = height / 2;
	b.v.x = b.speed;
	b.v.y = 0;
}

	bool Pong::Intersect(double x, double y, double w, double h, double ballx, double bally, double ballw, double ballh) {
		return x < ballx + ballw && y < bally + ballh && ballx < x + w && bally < y + h;
	} 
	//ballw,ballh width and height is the ball's radius*2

	void Pong::movePlayer(unsigned int user, unsigned int keyCode, unsigned int latency, unsigned int sequence_num)
	{
		player*pt = nullptr;
		playerMiddle*pt2 = nullptr;
		if (user == 0) {
			pt = &p;
			p.sequence_num = sequence_num;
		}
		else if (user == 1) {
			pt = &p2;
			p2.sequence_num - sequence_num;
		}
		else if (user == 2)
		{
			pt2 = &p3;
			p3.sequence_num - sequence_num;
		}
		else if (user == 3)
		{
			pt2 = &p4;
			p4.sequence_num - sequence_num;
		}
		if (pt2 != nullptr)
		{
			if (keyCode == 37)
			{
				pt2->x -= 15;
			}
			else if (keyCode == 39) {
				pt2->x += 15;
			}
			pt2->x = fmax(fmin(pt2->x, width - pt2->width), 0);
			pt2->latency = latency;
		}
		else if (pt != nullptr)
		{
			if (keyCode == 38)
			{
				pt->y -= 15;
			}
			else if (keyCode == 40) {
				pt->y += 15;
			}
			pt->y = fmax(fmin(pt->y, height - pt->height), 0);
			pt->latency = latency;
		}
	}
	int attack = 0;
	bool Pong::update()
	{	
		//imititation latency
		if ((p.latency > 200 && b.v.x < 0 && b.x < width / 2) || (p2.latency > 200 && b.v.x > 0 && b.x > width / 2) || (p3.latency > 200 && b.v.x > 0 && b.x > width / 2) || (p4.latency > 200 && b.v.x > 0 && b.x > width / 2)) {

			b.x += b.v.x / 1.5;
			b.y += b.v.y / 1.5;
		}
		else
		{
			b.x += b.v.x;
			b.y += b.v.y;
		}
		//ball bounce back when hit the wall
		if (b.radius > b.y || b.y + b.radius > height) {
			b.v.y *= -1;
		}
		//the third wall   without the wall at the paddle's side
		if (b.x + b.radius > width) {
			b.v.x *= -1;
		}

		double minAngle = -30;
		double maxAngle = 30;
		double angle = floor((rand() % (N + 1) / (float)(N + 1)) * (3.14));

		// ball hit the player and bounce back 
		if (Intersect(p.x, p.y, p.width, p.height, b.x - b.radius, b.y - b.radius, b.radius * 2, b.radius * 2)) {
			double n = (b.y + b.radius - p.y) / (p.height + b.radius);
			double theta = M_PI / 4 * (2 * n - 1);
			double smash = abs(theta) > .1 * M_PI ? 1.5 : 1;
			b.v.x = smash * b.speed * cos(theta);
			b.v.y = smash * b.speed * sin(theta);
			attack = 1;
		}

		if (Intersect(p2.x, p2.y, p2.width, p2.height, b.x - b.radius, b.y - b.radius, b.radius * 2, b.radius * 2)) {
			double n = (b.y + b.radius - p2.y) / (p2.height + b.radius);
			double theta = M_PI / 4 * (2 * n - 1);
			//double smash = abs(theta) > .1 * M_PI ? 1.5 : 1;
			double smash = 1; //removed smash
			b.v.x = -1 * smash * b.speed * cos(theta);
			b.v.y = -1 * smash * b.speed * sin(theta);
			attack = 2;
		}
		if (Intersect(p3.x, p3.y, p3.width, p3.height, b.x - b.radius, b.y - b.radius, b.radius * 2, b.radius * 2)) {
			double n = (b.x + b.radius - p3.x) / (p3.width + b.radius);
			double theta = M_PI / 4 * (2 * n - 1);
			//double smash = abs(theta) > .1 * M_PI ? 1.5 : 1;
			double smash = 1; //removed smash
			b.v.x = 1 * smash * b.speed * cos(theta);
			b.v.y = 1 * smash * b.speed * sin(theta);
			attack = 3;
		}
		if (Intersect(p4.x, p4.y, p4.width, p4.height, b.x - b.radius, b.y - b.radius, b.radius * 2, b.radius * 2)) {
			double n = (b.x + b.radius - p4.x) / (p4.width + b.radius);
			double theta = M_PI / 4 * (2 * n - 1);
			//double smash = abs(theta) > .1 * M_PI ? 1.5 : 1;
			double smash = 1; //removed smash
			b.v.x = -1 * smash * b.speed * cos(theta);
			b.v.y = 1 * smash * b.speed * sin(theta);
			attack = 4;
		}



		//*****keep track of consequetive hits :score*****
		// right side player #2
		if (b.x + b.radius > width) {
			if (attack == 1) {
				++s.p;
			}
			if (attack == 3) { ++s.p3;}
			if (attack == 4) { ++s.p4; }
			
				init();
			return true;
		}
		//bottom side player #4
		if (b.y + b.radius > height) {
			if (attack == 1) {
				++s.p;
			}
			if (attack == 3) { ++s.p3; }
			if (attack == 2) { ++s.p2; }
			
				init();
			return true;
		}
		//top  player #3
		if (b.radius>b.y ) {
			
			if (attack == 1) {
				++s.p;
			}
			if (attack == 2) { ++s.p2; }
			if (attack == 4) { ++s.p4; }
				init();
			return true;
		}
		//left player #1
		if (b.x + b.radius < 0) {
			std::cout << attack;
			if (attack == 2) {
				++s.p2;
			}
			if (attack == 3) { ++s.p3; }
			if (attack == 4){++s.p4;}
				init();
			return true;
		}
		return false;
	}
	std::ostringstream Pong::trackmovement() {
		std::ostringstream os;
		os << p.x << "," << p.y << "," << p2.x << "," << p2.y << ","<< p3.x << "," << p3.y << ","<< p4.x << "," << p4.y << ","<< b.x << "," << b.y << "," << s.p<<"," << s.p2 << "," << s.p3 << "," << s.p4 << "," << p.latency << "," << p2.latency << "," << p3.latency << "," << p4.latency  << "," << p.sequence_num << "," << p2.sequence_num << "," << p3.sequence_num << "," << p4.sequence_num; 
		return os;
	}

	std::vector<unsigned int> Pong::Ping_output()
	{
		std::vector<unsigned int> Pings;
		Pings.push_back(p.latency);
		Pings.push_back(p2.latency);
		Pings.push_back(p3.latency);
		Pings.push_back(p4.latency);
		return Pings;
	}
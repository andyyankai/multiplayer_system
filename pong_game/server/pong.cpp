#include "pong.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <sstream>


Pong::Pong(unsigned int width, unsigned int height) {
	this->width = width;
	this->height = height;
	init();
}

Pong::~Pong(){}
void Pong::init() {
	p.x = p.width;
	p.y = (height - p.height) / 2;

	b.x = width / 2;
	b.y = height / 2;
	b.v.x = b.speed;
	b.v.y = 0;
}

	bool Pong::Intersect(double x, double y, double w, double h, double ballx, double bally, double ballw, double ballh) {
		return x < ballx + ballw && y < bally + ballh && ballx < x + w && bally < y + h;
	} 
	//ballw,ballh width and height is the ball's radius*2

	void Pong::movePlayer(unsigned int user, unsigned int keyCode)
	{
		player*pt = nullptr;
		if (user == 0) {
			pt = &p;
		}
		if (pt != nullptr)
		{
			if (keyCode == 38)
			{
				pt->y -= 15;
			}
			else if (keyCode == 40) {
				pt->y += 15;
			}
			pt->y = fmax(fmin(pt->y, height - pt->height), 0);

		}
	}
	bool Pong::update()
	{	
		b.x += b.v.x;
		b.y += b.v.y;
		//ball bounce back when hit the wall
		if (b.radius > b.y || b.y + b.radius > height) {
			b.v.y *= -1;
		}
		//the third wall   without the wall at the paddle's side
		if (b.x + b.radius > width) {
			b.v.x *= -1;
		}

		if (Intersect(p.x, p.y, p.width, p.height, b.x - b.radius, b.y - b.radius, b.radius * 2, b.radius * 2)) {
			double n = (b.y + b.radius - p.y) / (p.height + b.radius);
			double theta = M_PI / 4 * (2 * n - 1);
			double smash = abs(theta) > .1 * M_PI ? 1.5 : 1;
			b.v.x = smash * b.speed * cos(theta);
			b.v.y = smash * b.speed * sin(theta);
		}

		//*****keep track of consequetive hits :score*****
		if (b.x + b.radius > width) {
		
			++s.p;
			return true;
		}
		//hit second wall score
		if (b.y + b.radius > height) {
			++s.p;
			return true;
		}
		//hit third wall
		if (b.radius>b.y ) {
			++s.p;
			return true;
		}
		//lost ball condition
		if (b.x + b.radius < 0) {
			//restarts game  the ball hits the boarder behind the player (left)
			init();
			--s.p;
			return true;
		}
		return false;
	}
	std::ostringstream Pong::trackmovement() {
		std::ostringstream os;
		os << p.x << "," << p.y << "," << b.x << "," << b.y << "," << s.p ;
		return os;
	}

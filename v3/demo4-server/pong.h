#ifndef PONG_H
#define PONG_H
#include <string>
class Pong{
public:
	Pong(unsigned int width, unsigned int height) ;
	~Pong();
	void init();
	void movePlayer(unsigned int user, unsigned int keyCode) ;
	bool update();
	std::ostringstream trackmovement();

private:
	bool Intersect(double x, double y, double w, double h, double ballx, double bally, double ballw, double ballh);

	unsigned int width;
	unsigned int height;

	struct player {
		double x;
		double y;
		struct velocity {
			double x;
			double y;
		};
		velocity v;
		unsigned int width = 20;
		unsigned int height = 100;
	
	};
	struct playerMiddle {
		double x;
		double y;
		struct velocity {
			double x;
			double y;
		};
		velocity v;
		unsigned int width = 100;
		unsigned int height = 20;

	};

	struct ball {
		double x;
		double y;
		struct velocity {
			double x;
			double y;
		};
		velocity v;
		double speed = 2;
		double radius = 20;
	};

	struct score {
		unsigned int p = 0;
		unsigned int p2 = 0;
		unsigned int p3 = 0;
		unsigned int p4 = 0;
		
	};

	player p;
	player p2;
	playerMiddle p3;
	playerMiddle p4;
	ball b;
	score s;
};



#endif
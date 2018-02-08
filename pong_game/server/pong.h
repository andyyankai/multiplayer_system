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
		unsigned int width = 20;
		unsigned int height = 100;
	
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
		
	};

	player p;
	ball b;
	score s;
};



#endif

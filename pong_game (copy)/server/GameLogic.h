#pragma once
#include <string>

#include <math.h>
#include <iostream>
#define ballSpeedGL		2.5f
#define reducedSpeed    2.0f
#define paddleSpeedGL	0.5f
#define NBoundGL		10
#define SBoundGL		890
#define EBoundGL		390
#define WBoundGL		10

#define BALLSTARTX		450;
#define BALLSTARTY		200;

class GameLogic {

public:
	GameLogic();
	~GameLogic();

	void Reinitialize();
	std::string update(float px, float py, float px1, float py1, float px2, float py2, float px3, float py3);
	bool checkCollision(float bx, float by, float px, float py, float px1, float py1, float px2, float py2, float px3, float py3);
	void applyMotion();

	bool bounceBottom;
	bool bounceTop;

	bool isBallSpeedReduced;

	float bx;
	float by;

	int padX, padY, padX2, padY2, padX3, padY3, padX4, padY4;

	int width;
	int height;

	int player1Latency;
	int player2Latency;
	int player3Latency;
	int player4Latency;
	int latencyAverage;

	int state;

private:

	float hits;
	int hitRatio;
	
	int p1Score, p2Score, p3Score, p4Score;
	struct velocity {
			float x;
			float y;
		};
		velocity ballVelocity;
		
	int attempts;
	int playerID;

};

enum DIRECTION
{
	HIT_RIGHTSIDE_TOP,
	HIT_LEFTSIDE_TOP,
	HIT_LEFTSIDE_BOTTOM,
	HIT_RIGHTSIDE_BOTTOM,
	HIT_PADDLE_LEFT,
	HIT_PADDLE_RIGHT
};

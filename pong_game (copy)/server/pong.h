#ifndef PONG_H
#define PONG_H
#include <string>
class GameLogic {

public:
	GameLogic();
	~GameLogic();

	void Reinitialize();
	std::string update(float px, float py, float px1, float py1);
	bool checkCollision(float bx, float by, float px, float py, float px1, float py1);
	void applyMotion();

	bool bounceBottom;
	bool bounceTop;

	bool isBallSpeedReduced;

	float bx;
	float by;

	int padX, padY, padX2, padY2;

	int width;
	int height;

	int playerOneLatency;
	int playerTwoLatency;
	int playerThreeLatency;
	int playerFourLatency;
	int latencyAverage;

	int state;

private:

	float hits;
	int hitRatio;
	int pOneScore, pTwoScore, pThreeScore, pFourScore;
	sf::Vector2f ballVelocity;
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

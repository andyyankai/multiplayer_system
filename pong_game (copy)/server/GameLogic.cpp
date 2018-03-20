#include "GameLogic.h"


double round(double d)
{
  return floor(d + 0.5);
}

GameLogic::GameLogic()
{
	velocity temp{ballSpeedGL, -ballSpeedGL};
	
	ballVelocity = temp;
	state = DIRECTION::HIT_PADDLE_LEFT;
	hits = 0.0f;
	attempts = 0;
	//Initialize all values
	height = 900;
	width = 900;
	bx = 450;
	by = 450;
	bounceBottom = false;
	bounceTop = false;
}

GameLogic::~GameLogic(){}

void GameLogic::Reinitialize()
{
	velocity temp{ballSpeedGL, -ballSpeedGL};
	ballVelocity = temp;
	state = DIRECTION::HIT_PADDLE_LEFT;
	hits = 0.0f;
	attempts = 0;
	//Initialize all values
	player1Latency = 0;
	player2Latency = 0;
	player3Latency = 0;
	player4Latency = 0;
	height = 900;
	width = 900;
	bx = 450;
	by = 450;
	bounceBottom = false;
	bounceTop = false;
	isBallSpeedReduced = false;
	p1Score = 0;
	p2Score = 0;
	p3Score = 0;
	p4Score = 0;
}

void GameLogic::applyMotion()
{
	switch (state){

	case DIRECTION::HIT_LEFTSIDE_TOP:
	{
		bx += ballVelocity.x;
		by += ballVelocity.y;
		break;
	}
	case DIRECTION::HIT_LEFTSIDE_BOTTOM:
	{
		bx += ballVelocity.x;
		by -= ballVelocity.y;
		break;
	}
	case DIRECTION::HIT_RIGHTSIDE_TOP:
	{
		bx -= ballVelocity.x;
		by += ballVelocity.y;
		break;
	}
	case DIRECTION::HIT_RIGHTSIDE_BOTTOM:
	{
		bx -= ballVelocity.x;
		by -= ballVelocity.y;
		break;
	}
	case DIRECTION::HIT_PADDLE_LEFT:
	{
		if (bounceBottom)
		{
			state = DIRECTION::HIT_LEFTSIDE_BOTTOM;
			bx += rand() % 5 + 1;
			bounceBottom = false;
		}
		else if (bounceTop)
		{
			state = DIRECTION::HIT_LEFTSIDE_TOP;
			bx += rand() % 5 + 1;
			bounceTop = false;
		}
		bx += ballVelocity.x;
		break;
	}
	case DIRECTION::HIT_PADDLE_RIGHT:
	{
		if (bounceBottom)
		{
			state = DIRECTION::HIT_RIGHTSIDE_BOTTOM;
			bx -= rand() % 5 + 1;
			bounceBottom = false;
		}
		else if (bounceTop)
		{
			state = DIRECTION::HIT_RIGHTSIDE_TOP;
			bx -= rand() % 5 + 1;
			bounceTop = false;
		}
		bx -= ballVelocity.x;
		break;
	}
	default: 
		break;

	}
}

std::string GameLogic::update(float px, float py, float px1, float py1, float px2, float py2, float px3, float py3)
{
	padX = px;
	padY = py;

	padX2 = px1;
	padY2 = py1;
	
	padX3 = px;
	padY3 = py;
	
	padX4 = px;
	padY4 = py;
	
	// top

	if (by + 5 < 5)
	{
		state = DIRECTION::HIT_PADDLE_LEFT;
		bx = 450;
		by = 450;
		velocity temp{ballSpeedGL, -ballSpeedGL};
		ballVelocity = temp;
		p1Score++;
	}

	// bottom
	if (by + 5 > height)
	{
		state = DIRECTION::HIT_PADDLE_LEFT;
		bx = 450;
		by = 450;
		velocity temp{ballSpeedGL, -ballSpeedGL};
		ballVelocity = temp;
		p2Score++;
	}

	// right
	if (bx + 5 > width)
	{
		state = DIRECTION::HIT_PADDLE_LEFT;
		bx = 450;
		by = 450;
		velocity temp{ballSpeedGL, -ballSpeedGL};
		ballVelocity = temp;
		p1Score++;
	}

	// left
	if (bx + 5 < 5)
	{
		state = DIRECTION::HIT_PADDLE_LEFT;
		bx = 450;
		by = 450;
		velocity temp{ballSpeedGL, -ballSpeedGL};
		ballVelocity = temp;
		p1Score++;
	}

	latencyAverage = (player1Latency + player2Latency + player3Latency + player4Latency) / 4;
	if (bx <= 200 && latencyAverage >= 50 && (state == DIRECTION::HIT_RIGHTSIDE_TOP || state == DIRECTION::HIT_RIGHTSIDE_BOTTOM))
	{
		isBallSpeedReduced = true;
		velocity temp{reducedSpeed, -reducedSpeed};
		ballVelocity = temp;
	}
	else if (bx >= 700 && latencyAverage >= 50 && (state == DIRECTION::HIT_LEFTSIDE_TOP || state == DIRECTION::HIT_LEFTSIDE_BOTTOM))
	{
		isBallSpeedReduced = true;
		velocity temp{reducedSpeed, -reducedSpeed};
		ballVelocity = temp;
	}


	applyMotion();
	if (checkCollision(bx, by, px, py, px1, py1, px2, py2, px3, py3) && isBallSpeedReduced)
	{
		velocity temp{ballSpeedGL, -ballSpeedGL};
		ballVelocity = temp;
		isBallSpeedReduced = false;
	}

	std::string returnData;
	returnData = std::to_string(bx) + "," + std::to_string(by) + "," + 
				 std::to_string(p1Score) + "," + std::to_string(p2Score) + "," + std::to_string(p3Score) + "," + std::to_string(p4Score);

	return returnData;
}

bool GameLogic::checkCollision(float bx, float by, float px, float py, float px1, float py1, float px2, float py2, float px3, float py3)
{

	if (by >= py1 && by <= py1 + 30 && bx + 10 > 875)
	{
		bounceTop = true;
		state = DIRECTION::HIT_PADDLE_RIGHT;
		return true;
	}
	//Hit the center
	else if (by >= py1 + 30 && by <= py1 + 50 && bx + 10 > 875)
	{
		state = DIRECTION::HIT_PADDLE_RIGHT;
		return true;
	}//Hit the bottom of the paddle
	else if (by >= py1 + 50 && by <= py1 + 80 && bx + 10 > 875)
	{
		state = DIRECTION::HIT_PADDLE_RIGHT;
		bounceBottom = true;
		return true;
	}

	// PLAYER 1's PADDLE - ON THE LEFT SIDE

	else if (by >= py && by <= py + 30 && bx < 25)
	{
		bounceTop = true;
		state = DIRECTION::HIT_PADDLE_LEFT;
		return true;
	}//Hit the center
	else if (by >= py +30 && by <= py + 50 && bx < 25)
	{
		state = DIRECTION::HIT_PADDLE_LEFT;
		return true;
	}//Hit the bottom of the paddle
	else if (by >= py + 50 && by <= py + 80 && bx < 25)
	{
		state = DIRECTION::HIT_PADDLE_LEFT;
		bounceBottom = true;
		return true;
	}
	
	

	return false;
}

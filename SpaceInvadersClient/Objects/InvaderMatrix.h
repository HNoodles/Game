#pragma once
#include "GameObject.h"
#include "Invader.h"
#include "Bullet.h"

class InvaderMatrix
{
private:
	vector<vector<Invader*>> invaders;

	float leftBound, rightBound;
	double timeCount;
	int roundCount;
	vector<bool> heading;// left or right, down or not

	const double MOVE_STEP = 2.0f, TIME_STEP = 0.5f;

	list<Bullet*> bullets;
	const int MAX_FIRE_NUM = 3;// max 3 bullets at a time

	float getLeftPos();
	
	float getRightPos();

	void setHeading();

	void fire();

	Invader* randomlySelectOne();
public:
	InvaderMatrix(EventManager* manager, int row, int column, 
		Vector2f topleft, Vector2f range, float moveRange, 
		Vector2f velocity, Timeline& timeline);

	~InvaderMatrix();

	void move(double elapsed);

	vector<bool> getHeading() const
	{
		return heading;
	}
};


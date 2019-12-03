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

	const double MOVE_STEP = 2.0f, TIME_STEP = 1.0f;

	list<Bullet*> bullets;
	const int MAX_FIRE_NUM = 2;// max 3 bullets at a time

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

	void kill(Invader* invader);

	vector<bool> getHeading() const
	{
		return heading;
	}

	list<Collidable*> getInvaderCList()
	{
		list<Collidable*> list;

		for (vector<Invader*> row : invaders)
		{
			for (Invader* invader : row)
			{
				list.push_back(
					dynamic_cast<Collidable*>(invader->getGC(ComponentType::COLLIDABLE))
				);
			}
		}

		return list;
	}

	list<Collidable*> getBulletsCList()
	{
		list<Collidable*> list;

		for (Bullet* bullet : bullets)
		{
			list.push_back(
				dynamic_cast<Collidable*>(bullet->getGC(ComponentType::COLLIDABLE))
			);
		}

		return list;
	}

	list<Renderable*> getRList() const
	{
		list<Renderable*> list;

		for (vector<Invader*> row : invaders)
		{
			for (Invader* invader : row)
			{
				list.push_back(
					dynamic_cast<Renderable*>(invader->getGC(ComponentType::RENDERABLE))
				);
			}
		}
		for (Bullet* bullet : bullets)
		{
			list.push_back(
				dynamic_cast<Renderable*>(bullet->getGC(ComponentType::RENDERABLE))
			);
		}

		return list;
	}
};

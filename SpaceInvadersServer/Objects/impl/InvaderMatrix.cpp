#include "../InvaderMatrix.h"

float InvaderMatrix::getLeftPos()
{
	// pick first element of every row, find out the least pos x
	float posx = 1000.f;
	for (vector<Invader*> row : invaders)
	{
		if (row.size() > 0)
		{
			float newPos = dynamic_cast<Renderable*>((*row.begin())->getGC(ComponentType::RENDERABLE))
				->getShape()->getPosition().x;
			if (newPos < posx)
			{
				posx = newPos;
			}
		}
	}
	return posx;
}

float InvaderMatrix::getRightPos()
{
	// pick last element of every row, find out the least pos x
	float posx = -1000.f;
	for (vector<Invader*> row : invaders)
	{
		if (row.size() > 0)
		{
			float newPos = dynamic_cast<Renderable*>((*(row.end() - 1))->getGC(ComponentType::RENDERABLE))
				->getShape()->getPosition().x;
			if (newPos > posx)
			{
				posx = newPos;
			}
		}
	}
	return posx;
}

void InvaderMatrix::setHeading()
{
	bool hitLeft = getLeftPos() <= leftBound;
	bool hitRight = getRightPos() >= rightBound;

	if (!heading[1] && // last time not going down
		((heading[0] && hitRight) || (!heading[0] && hitLeft))) // moving right hit right, or moving left hit left
	{// go down
		heading[1] = true;
	}
	else if ((heading[0] && !hitRight) || (heading[1] && hitLeft)) // moving right not hit right, or moving down at left
	{// go right
		heading = { true, false };
	}
	else if ((!heading[0] && !hitLeft) || (heading[1] && hitRight)) // moving left not hit left, or moving down at right
	{// go left
		heading = { false, false };
	}
}

Invader* InvaderMatrix::randomlySelectOne()
{
	int row = rand() % invaders.size(), column = 0;

	while (invaders[row].size() == 0)
	{
		row = rand() % invaders.size();
	}

	column = rand() % invaders[row].size();
	
	return invaders[row][column];
}

bool InvaderMatrix::checkWin()
{
	int totalsize = 0;
	for (vector<Invader*> row : invaders)
	{
		totalsize += row.size();
	}
	return totalsize == 0;
}

InvaderMatrix::InvaderMatrix(EventManager* manager, int row, int column,
	Vector2f topleft, Vector2f range, float moveRange, 
	Vector2f velocity, Timeline& timeline) : 
	leftBound(topleft.x), rightBound(topleft.x + moveRange), 
	timeCount(0), roundCount(0), heading({true, false}), win(false)
{
	Vector2f step_size = Vector2f(range.x / column, range.y / row);

	for (int i = 0; i < row; i++) 
	{
		vector<Invader*> row;
		for (int j = 0; j < column; j++)
		{
			Vector2f pos = Vector2f(topleft.x + j * step_size.x, topleft.y + i * step_size.y);

			row.push_back(
				new Invader("I" + to_string(i) + to_string(j), manager, pos, velocity, timeline, this)
			);
		}
		invaders.push_back(row);
	}
}

InvaderMatrix::~InvaderMatrix()
{
	// delete remaining invaders
	for (vector<Invader*> row : invaders)
	{
		for (Invader* invader : row)
		{
			delete invader;
			invader = nullptr;
		}
	}
	for (Invader* invader : killed)
	{
		delete invader;
		invader = nullptr;
	}

	// delete remaining bullets
	for (Bullet* bullet : bullets)
	{
		delete bullet;
		bullet = nullptr;
	}
	for (Bullet* bullet : expired)
	{
		delete bullet;
		bullet = nullptr;
	}
}

void InvaderMatrix::move(double elapsed)
{
	// move old bullets first
	for (Bullet* bullet : bullets)
	{
		dynamic_cast<Movable*>(bullet->getGC(ComponentType::MOVABLE))->work(elapsed);
	}

	timeCount += elapsed;

	// only move when time count >= time step
	if (timeCount < TIME_STEP)
	{
		return;
	}

	// time count >= time step, move, reset time count
	timeCount -= TIME_STEP;
	roundCount++;

	// check if is winning
	win = checkWin();
	if (win)
	{
		return;
	}

	// determine direction to move
	setHeading();

	// move all invaders
	for (vector<Invader*> row : invaders)
	{
		for (Invader* invader : row)
		{
			dynamic_cast<Movable*>(invader->getGC(ComponentType::MOVABLE))->work(TIME_STEP);
		}
	}

	// fire bullets
	fire();
}

void InvaderMatrix::kill(Invader* invader)
{
	// locate invader
	string id = invader->getId();
	int row = atoi(id.substr(1, 1).c_str());
	int column = atoi(id.substr(2, 1).c_str());

	// remove from matrix
	invaders[row].erase(find(invaders[row].begin(), invaders[row].end(), invader));

	// put into killed
	killed.push_back(invader);
}

void InvaderMatrix::fire()
{
	// randomly select fire invaders
	vector<Invader*> firing;
	int fire_num = rand() % MAX_FIRE_NUM;
	for (int i = 0; i < fire_num; i++)
	{
		firing.push_back(randomlySelectOne());
	}

	// create new bullets
	for (Invader* invader : firing)
	{
		bullets.push_back(invader->fire(roundCount));
	}

	for (auto iter = bullets.begin(); iter != bullets.end(); )
	{
		Vector2f pos = dynamic_cast<Renderable*>((*iter)->getGC(ComponentType::RENDERABLE))
			->getShape()->getPosition();

		if (pos.y > 600) // out of screen
		{
			// move bullet to expired
			expired.push_back((*iter));
			bullets.erase(iter++);
		}
		else
			iter++;
	}
}

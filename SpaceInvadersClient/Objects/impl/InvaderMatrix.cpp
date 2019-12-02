#include "../InvaderMatrix.h"

float InvaderMatrix::getLeftPos()
{
	return dynamic_cast<Renderable*>(invaders[0][0]->getGC(ComponentType::RENDERABLE))
		->getShape()->getPosition().x;
}

float InvaderMatrix::getRightPos()
{
	Renderable* renderable = dynamic_cast<Renderable*>(invaders[0][invaders[0].size() - 1]->getGC(ComponentType::RENDERABLE));
	return renderable->getShape()->getPosition().x + renderable->getSize().x;
}

void InvaderMatrix::setHeading()
{
	bool hitLeft = getLeftPos() <= leftBound;
	bool hitRight = getRightPos() >= rightBound;

	if (!heading[1] && // last time not going down
		(heading[0] && hitRight) || (!heading[0] && hitLeft)) // moving right hit right, or moving left hit left
	{// go down
		heading[1] = true;
	}

	if ((heading[0] && !hitRight) || (heading[1] && hitLeft)) // moving right not hit right, or moving down at left
	{// go right
		heading = { true, false };
	}

	if ((!heading[0] && !hitLeft) || (heading[1] && hitRight)) // moving left not hit left, or moving down at right
	{// go left
		heading = { false, false };
	}
}

Invader* InvaderMatrix::randomlySelectOne()
{
	int row = rand() % invaders.size(), column = 0;

	while (invaders[row].size() == 0)
	{
		column = rand() % invaders[row].size();
	}
	
	return invaders[row][column];
}

InvaderMatrix::InvaderMatrix(EventManager* manager, int row, int column,
	Vector2f topleft, Vector2f range, float moveRange, 
	Vector2f velocity, Timeline& timeline) : 
	leftBound(topleft.x), rightBound(topleft.x + moveRange), timeCount(0), roundCount(0), heading({true, false})
{
	Vector2f step_size = Vector2f(range.x / column, range.y / row);

	for (int i = 0; i < row; i++) 
	{
		vector<Invader*> row;
		for (int j = 0; j < column; j++)
		{
			Vector2f pos = Vector2f(topleft.x + j * step_size.x, topleft.y + i * step_size.y);

			row.push_back(
				new Invader(to_string(i) + to_string(j), manager, pos, velocity, timeline, this)
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

	// delete remaining bullets
	for (Bullet* bullet : bullets)
	{
		delete bullet;
		bullet = nullptr;
	}
}

void InvaderMatrix::move(double elapsed)
{
	timeCount += elapsed;

	// only move when time count >= time step
	if (timeCount < TIME_STEP)
	{
		return;
	}

	// time count >= time step, move, reset time count
	timeCount -= TIME_STEP;
	roundCount++;

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

void InvaderMatrix::fire()
{
	// randomly select fire invaders
	vector<Invader*> firing;
	int fire_num = rand() % MAX_FIRE_NUM + 1;
	for (int i = 0; i < fire_num; i++)
	{
		firing.push_back(randomlySelectOne());
	}

	// create new bullets
	for (Invader* invader : firing)
	{
		bullets.push_back(invader->fire(roundCount));
	}

	// delete expired bullets
	for (Bullet* bullet : bullets)
	{
		Vector2f pos = dynamic_cast<Renderable*>(bullet->getGC(ComponentType::RENDERABLE))
			->getShape()->getPosition();

		if (pos.y > 600) // out of screen
		{
			// delete bullet and set to null
			delete bullet;
			bullet = nullptr;
		}
	}
	// remove all nullptrs in bullets
	for (auto iter = bullets.begin(); iter != bullets.end(); )
	{
		if (*iter == nullptr)
		{
			bullets.erase(iter++);
		}
		else
		{
			iter++;
		}
	}
}

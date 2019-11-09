#include "../EventManager.h"

EventManager::EventManager(Timeline& gameTime, mutex* mtxObjMov)
	: gameTime(gameTime), handler(gameTime, this), GVT(gameTime.getTime()), mtxObjMov(mtxObjMov)
{
	addQueue(SELF_NAME);
	insertGVT(SELF_NAME, GVT);
}

EventManager::~EventManager()
{
	// delete all stored event pointers
	for (auto pair : queues)
	{
		auto queue = pair.second;
		while (!queue.empty())
		{
			delete queue.top();
			queue.pop();
		}
	}
}

void EventManager::executeEvents()
{
	for (auto pair : queues)
	{ // go through each queue
		auto queue = pair.second;

		// handle events on top of queue if execution time <= GVT
		while (queue.top()->getExecuteTime() <= GVT)
		{
			const ::Event* e = queue.top();

			// handle event
			if (e->getType() == Event_t::OBJ_MOVEMENT)
				mtxObjMov->lock();
			handler.onEvent(e);
			if (e->getType() == Event_t::OBJ_MOVEMENT)
				mtxObjMov->unlock();
			// delete the pointer
			delete e;
			// pop it from the queue
			queue.pop();
		}
	}	
}

void EventManager::updateGVT()
{
	double temp = getRequestGVT();

	// find the smallest claimed GVT
	for (auto pair : GVTs)
	{
		if (pair.second < temp)
			temp = pair.second;
	}

	GVT = temp;
}

#include "../EventManager.h"

EventManager::EventManager(Timeline* gameTime, mutex* mtxObjMov, Replay* replay)
	: gameTime(gameTime), handler(gameTime, this), GVT(gameTime->getTime()), 
	mtxObjMov(mtxObjMov), connected(true), replaying(false), replay(replay)
{
	addQueue(SELF_NAME);
}

EventManager::~EventManager()
{
	// delete all stored event pointers
	for (auto& pair : queues)
	{
		auto& queue = pair.second;
		while (!queue.empty())
		{
			delete queue.top();
			queue.pop();
		}
	}
}

void EventManager::executeEvents()
{
	updateGVT();

	for (auto& pair : queues)
	{ // go through each queue
		auto& queue = pair.second;

		// handle events on top of queue if execution time <= GVT
		while (!queue.empty() && queue.top()->getExecuteTime() <= GVT)
		{
			const ::Event* e = queue.top();

			// handle event
			bool isEndRec = e->getType() == Event_t::END_REC;
			bool isObjMov = e->getType() == Event_t::OBJ_MOVEMENT;
			if (isObjMov)
				mtxObjMov->lock();
			handler.onEvent(e);
			if (isObjMov)
				mtxObjMov->unlock();

			// queue will be empty after handling end rec event
			// thus, avoid deleting and poping
			if (isEndRec)
				continue;

			// delete pointer
			delete e;

			// pop it from the queue
			queue.pop();
		}
	}
}

void EventManager::keepExecutingEvents()
{
	while (connected)
	{
		lock_guard<mutex> guard(mtxQueue);
		executeEvents();
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

void EventManager::clearEvents()
{
	// delete all stored event pointers
	for (auto& pair : queues)
	{
		auto& queue = pair.second;
		while (!queue.empty())
		{
			delete queue.top();
			queue.pop();
		}
	}
}

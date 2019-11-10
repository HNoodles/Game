#include "../EventManager.h"

EventManager::EventManager(Timeline& gameTime)
	: gameTime(gameTime), handler(gameTime, this), GVT(gameTime.getTime())
{
	addQueue(SELF_NAME);
}

EventManager::~EventManager()
{
	// delete all stored event pointers
	for (auto pair : queues)
	{
		auto queue = pair.second;
		while (!queue.empty())
		{
			queue.pop();
		}
	}
}

void EventManager::executeEvents()
{
	for (auto pair : queues)
	{ // go through each queue
		//mtxQueue.lock();
		auto queue = pair.second;

		// handle events on top of queue if execution time <= GVT
		while (!queue.empty() && queue.top()->getExecuteTime() <= GVT)
		{
			const ::Event* e = queue.top();

			// handle event
			handler.onEvent(e);
			// pop it from the queue
			queue.pop();
		}

		// replace the queue in the map with new queue
		queues[pair.first] = queue;

		//mtxQueue.unlock();
	}	
}

void EventManager::updateGVT()
{
	// get the smallest execution time of own
	double temp = getRequestGVT();

	// find the smallest claimed GVT
	for (auto pair : GVTs)
	{
		if (pair.second < temp)
			temp = pair.second;
	}

	GVT = temp;
}

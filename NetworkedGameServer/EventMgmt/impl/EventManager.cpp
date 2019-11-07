#include "../EventManager.h"

EventManager::EventManager(GameTime& gameTime, const char selfName)
	: gameTime(gameTime), handler(gameTime), GVT(gameTime.getTime())
{
	addQueue(selfName);
	insertGVT(selfName, GVT);
}

void EventManager::executeEvents()
{
	for (auto pair : queues)
	{ // go through each queue
		auto queue = pair.second;

		// handle events on top of queue if execution time <= GVT
		while (queue.top().getExecuteTime() <= GVT)
		{
			const ::Event* e = &(queue.top());
			handler.onEvent(e);
			
			// store the event for publishing if is object movement event
			if (e->getType() == ::Event_t::OBJ_MOVEMENT)
				objMovements.push_back(*(EObjMovement*)e);
			
			queue.pop();
		}
	}	
}

void EventManager::updateGVT()
{
	double temp = gameTime.getTime();

	// find the smallest claimed GVT
	for (auto pair : GVTs)
	{
		if (pair.second < temp)
			temp = pair.second;
	}

	GVT = temp;
}

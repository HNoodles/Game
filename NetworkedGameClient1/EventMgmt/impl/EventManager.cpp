#include "../EventManager.h"

EventManager::EventManager(Timeline& gameTime, mutex* mtxObjMov)
	: gameTime(gameTime), handler(gameTime, this), GVT(gameTime.getTime()), mtxObjMov(mtxObjMov)
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
			delete queue.top();
			queue.pop();
		}
	}
}

void EventManager::executeEvents()
{
	list<const ::Event*> eventsToDelete;
	for (auto& pair : queues)
	{ // go through each queue
		//mtxQueue.lock();
		auto& queue = pair.second;

		// handle events on top of queue if execution time <= GVT
		while (!queue.empty() && queue.top()->getExecuteTime() <= GVT)
		{
			const ::Event* e = queue.top();

			// handle event
			bool isObjMov = e->getType() == Event_t::OBJ_MOVEMENT;
			if (isObjMov)
				mtxObjMov->lock();
			handler.onEvent(e);
			if (isObjMov)
				mtxObjMov->unlock();
			// delete pointer
			
			if (e != queue.top())
			{
				queue.push(new ECharDeath(gameTime.getTime(), nullptr));
				cout << "wrong!!!!!!" << endl;
				cout << to_string(e->getExecuteTime()) << endl;
				cout << to_string(queue.top()->getExecuteTime()) << endl;
				continue;
			}
			
			//delete e;
			/*if (!eventsToDelete.empty() && e != eventsToDelete.back())
			{*/
			eventsToDelete.emplace_back(e);
			/*}*/


			// pop it from the queue
			queue.pop();
		}
		//mtxQueue.unlock();
	}
	for (const ::Event* e : eventsToDelete)
	{
		delete e;
	}
	eventsToDelete.clear();
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

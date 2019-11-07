#pragma once
#include <queue>
#include "Event.h"
#include "EventHandler.h"

class EventManager
{
private:
	GameTime& gameTime;
	const char selfName;
	double GVT;
	EventHandler handler;
	map <
		const char, // client name
		priority_queue<::Event*, vector<::Event*>, greater<::Event*>>
	> queues;
	map <const char, double> GVTs;
	list<EObjMovement> objMovements;
public:
	EventManager(GameTime& gameTime, const char selfName);

	~EventManager();

	void executeEvents();

	void updateGVT();

	double getRequestGVT() const
	{
		return queues.find(selfName)->second.top()->getExecuteTime();
	}

	void insertGVT(const char client_name, double GVT)
	{
		GVTs.insert({ client_name, GVT });
	}

	void addQueue(const char client_name)
	{
		priority_queue<::Event*, vector<::Event*>, greater<::Event*>> newQueue;
		queues.insert({ client_name, newQueue });
	}

	void removeQueue(const char client_name)
	{
		queues.erase(client_name);
	}

	void insertEvent(const char client_name, ::Event* e)
	{
		queues.find(client_name)->second.push(e);

		// store the event for publishing if is object movement event
		if (e->getType() == ::Event_t::OBJ_MOVEMENT)
		{
			objMovements.push_back(*(EObjMovement*)e);
		}
	}

	list<EObjMovement>* getObjMovements()
	{
		return &objMovements;
	}
};


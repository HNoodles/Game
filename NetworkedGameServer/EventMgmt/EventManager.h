#pragma once
#include <queue>
#include "Event.h"
#include "EventHandler.h"

class EventManager
{
private:
	Timeline& gameTime;
	const char* const selfName;
	double GVT;
	EventHandler handler;
	map <
		const char* const, // client name
		priority_queue<::Event*, vector<::Event*>, greater<::Event*>>
	> queues;
	map <const char* const, double> GVTs;
	list<EObjMovement> objMovements;
public:
	EventManager(Timeline& gameTime, const char* const selfName);

	~EventManager();

	void executeEvents();

	void updateGVT();

	double getRequestGVT() const
	{
		return queues.find(selfName)->second.top()->getExecuteTime();
	}

	void insertGVT(const char* const client_name, double GVT)
	{
		GVTs.insert({ client_name, GVT });
	}

	void addQueue(const char* const client_name)
	{
		priority_queue<::Event*, vector<::Event*>, greater<::Event*>> newQueue;
		queues.insert({ client_name, newQueue });
	}

	void removeQueue(const char* const client_name)
	{
		queues.erase(client_name);
	}

	void insertEvent(const char* const client_name, ::Event* e)
	{
		// locate the pair
		auto pair = queues.find(client_name);
		
		// insert the event
		if (pair != queues.end())
		{
			pair->second.push(e);
		}
		else
		{
			addQueue(client_name);
			queues.find(client_name)->second.push(e);
		}

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


#pragma once
#include <queue>
#include <mutex>
#include "EventHandler.h"

const char* const SELF_NAME = "A";

class EventManager
{
private:
	Timeline& gameTime;
	double GVT;
	EventHandler handler;
	map <
		const char* const, // client name
		priority_queue<::Event*, vector<::Event*>, greater<::Event*>>
	> queues;
	map <const char* const, double> GVTs;
	list<EObjMovement> objMovements;
	mutex mtxEvt, *mtxObjMov;
public:
	EventManager(Timeline& gameTime, mutex* mtxObjMov);

	~EventManager();

	void executeEvents();

	void updateGVT();

	double getGVT() const
	{
		return GVT;
	}

	double getRequestGVT() const
	{
		auto queue = queues.find(SELF_NAME)->second;

		if (queue.empty()) // empty queue
		{
			return gameTime.getTime();
		}
		else
		{
			return queue.top()->getExecuteTime();
		}
	}

	void setGVT(double GVT)
	{
		this->GVT = GVT;
	}

	void insertGVT(const char* const client_name, double GVT)
	{
		GVTs.insert({ client_name, GVT });
	}

	void removeGVT(const char* const client_name)
	{
		GVTs.erase(client_name);
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

	void insertEvent(::Event* e, const char* const client_name = SELF_NAME)
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
			mtxEvt.lock();
			objMovements.push_back(*(EObjMovement*)e);
			mtxEvt.unlock();
		}
	}

	list<EObjMovement>* getObjMovements()
	{
		return &objMovements;
	}

	double getCurrentTime() const
	{
		return gameTime.getTime();
	}

	mutex* getMtxEvt()
	{
		return &mtxEvt;
	}
};


#pragma once
#include <queue>
#include <mutex>
#include "EventHandler.h"
#include "../Replay/Replay.h"

#include <iostream>

using namespace std;

const char* const SELF_NAME = "B";

class EventManager
{
private:
	Timeline* gameTime;
	double GVT;
	EventHandler handler;
	map <
		const char* const, // client name
		priority_queue<::Event*, vector<::Event*>, cmp>
	> queues;
	map <const char* const, double> GVTs;
	list<EObjMovement> objMovements;
	mutex mtxEvt, mtxQueue, *mtxObjMov;

	bool connected, replaying;
	Replay* replay;

	void updateGVT();

	void clearEvents();

	void clearGVTs()
	{
		GVTs.clear();
	}
public:
	EventManager(Timeline* gameTime, mutex* mtxObjMov, Replay* replay);

	~EventManager();

	void executeEvents();

	void keepExecutingEvents();

	void setTimeline(Timeline* timeline)
	{
		gameTime = timeline;
	}

	double getGVT() const
	{
		return GVT;
	}

	double getRequestGVT()
	{
		auto queue = queues.find(SELF_NAME)->second;
		double GVT;

		if (queue.empty()) // empty queue
		{
			GVT = gameTime->getTime();
			if (replaying) // add time bias when recording
				GVT += replay->getStartTime();
		}
		else
		{
			GVT = queue.top()->getExecuteTime();
		}

		return GVT;
	}

	void setGVT(double GVT)
	{
		this->GVT = GVT;
	}

	void insertGVT(const char* const client_name, double GVT)
	{
		GVTs[client_name] = GVT;
	}

	void removeGVT(const char* const client_name)
	{
		GVTs.erase(client_name);
	}

	void addQueue(const char* const client_name, 
		priority_queue<::Event*, vector<::Event*>, cmp>* newQueue = nullptr)
	{
		if (newQueue != nullptr) // provided a new queue
		{
			queues.insert({ client_name, *newQueue });
		}
		else // didn't provide a new queue
		{
			priority_queue<::Event*, vector<::Event*>, cmp> newQueue;
			queues.insert({ client_name, newQueue });
		}
	}

	void removeQueue(const char* const client_name)
	{
		queues.erase(client_name);
	}

	void insertEvent(::Event* e, const char* const client_name = SELF_NAME)
	{
		// locate the queue
		int count = queues.count(client_name);

		// insert the event
		if (count == 1)
		{
			queues[client_name].push(e);
		}
		else
		{
			addQueue(client_name);
			queues[client_name].push(e);
		}

		// store the event for publishing if is object movement event
		if (e->getType() == ::Event_t::OBJ_MOVEMENT)
		{
			// only store self movement events for networking
			if (client_name == SELF_NAME)
			{
				mtxEvt.lock();
				objMovements.push_back(*(EObjMovement*)e);
				mtxEvt.unlock();
			}

			// store the event for replaying if is recording
			if (replay->getIsRecording())
			{
				replay->pushEvent((EObjMovement*)e);
			}
		}
	}

	list<EObjMovement>* getObjMovements()
	{
		return &objMovements;
	}

	double getCurrentTime() const
	{
		return gameTime->getTime();
	}

	mutex* getMtxEvt()
	{
		return &mtxEvt;
	}

	mutex* getMtxQueue()
	{
		return &mtxQueue;
	}

	void setConnected(bool connected)
	{
		this->connected = connected;

		if (!connected) // disconnected
		{
			clearEvents();
		}
	}

	void setReplay(bool isReplay)
	{
		replaying = isReplay;

		if (isReplay) // prepare for replay
		{
			clearEvents();
			clearGVTs();
		}
		else // finish replay
		{
			removeQueue("R");
		}
	}
};


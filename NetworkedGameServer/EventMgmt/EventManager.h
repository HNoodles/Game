#pragma once
#include <queue>
#include "Event.h"
#include "EventHandler.h"

class EventManager
{
private:
	GameTime& gameTime;
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

	void insertEvent(const char client_name, ::Event* e);
};


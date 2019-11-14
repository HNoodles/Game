#pragma once
#include <queue>
#include "../Times/GameTime.h"
#include "../EventMgmt/EObjMovement.h"
#include "../EventMgmt/EEndPlaying.h"

using namespace std;

class EventManager;
class Client;

class Replay
{
private:
	Timeline* gameTime;
	GameTime replayTime;
	double startTime;
	bool isRecording, isPlaying;
	priority_queue<::Event*, vector<::Event*>, cmp> records;
	EventManager* manager;
	Client* client;
public:
	Replay(Timeline* gameTime, EventManager* manager = nullptr, Client* client = nullptr);

	~Replay();

	void startRecording();

	void endRecording();

	void startPlaying();

	void endPlaying();

	void pushEvent(EObjMovement* e)
	{
		records.push(new EObjMovement(*e));
	}

	void resetPlaySpeed(double speed);

	GameTime& getReplayTime()
	{
		return replayTime;
	}

	double getStartTime()
	{
		return startTime;
	}

	void setStartTime(double startTime)
	{
		this->startTime = startTime;
	}

	bool getIsRecording()
	{
		return isRecording;
	}

	bool getIsPlaying()
	{
		return isPlaying;
	}

	void setEM(EventManager* manager)
	{
		this->manager = manager;
	}

	void setClient(Client* client)
	{
		this->client = client;
	}
};


#pragma once

#include <zmq.hpp>
#include <string>
#include <iostream>
#ifndef _WIN32
#include <unistd.h>
#else
#include <windows.h>
#endif

#include "../EventMgmt/EventManager.h"
#include "../Times/LocalTime.h"
#include "../Objects/Character.h"
#include "../Objects/InvaderMatrix.h"

using namespace std;
using namespace zmq;

extern const char* const SELF_NAME;

class Server
{
private:
	context_t context;
	socket_t receiver;
	socket_t publisher;

	map<string, GameObject*>* objects;
	list<GameObject*> expired;

	map<string, double> connectTimes;
	EventManager* manager;

	string s_recv(socket_t& socket);

	void s_send(socket_t& socket, const string& string);

	void Split(const string& string, const std::string& separator, vector<std::string>& result);

	void connectHandler(const string& name, double time);

	void disconnectHandler(const string& name);
public:
	Server(map<string, GameObject*>* objects, EventManager* manager);

	~Server();

	void receiverHandler(GameTime* gameTime);

	void publisherHandler(InvaderMatrix* invaders);
};


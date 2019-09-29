#pragma once

#include <zmq.hpp>
#include <string>
#include <iostream>
#ifndef _WIN32
#include <unistd.h>
#else
#include <windows.h>
#endif

#include <list>
#include <map>
#include "../Times/GameTime.h"
#include "../Times/LocalTime.h"
#include "../Objects/MovingPlatform.h"
#include "../Objects/Character.h"

using namespace std;
using namespace zmq;

typedef struct {
	//string name;
	LocalTime time;
	float x;
	float y;
} Client;


class Server
{
private:
	context_t context;
	socket_t receiver;
	socket_t publisher;
	map<string, Client> clients;

	string s_recv(socket_t& socket);

	void s_send(socket_t& socket, const string& string);

	void Split(const string& string, const std::string& separator, vector<std::string>& result);

	string PlatformMessage(float x, float y);

	string ClientMessage(string name, float x, float y, double thisTime);

public:
	Server(list<MovingPlatform*>* platforms);

	void receiverHandler(GameTime* gameTime, map<string, Character*>* characters);

	void publisherHandler(list<MovingPlatform*>* platforms);
};


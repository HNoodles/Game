#pragma once

#include <zmq.hpp>
#include <string>
#include <iostream>
#ifndef _WIN32
#include <unistd.h>
#else
#include <windows.h>
#endif

#include "../Times/LocalTime.h"
#include "../Objects/Character.h"

using namespace std;
using namespace zmq;


class Server
{
private:
	context_t context;
	socket_t receiver;
	socket_t publisher;
	map<string, Character> characters;

	string s_recv(socket_t& socket);

	void s_send(socket_t& socket, const string& string);

	void Split(const string& string, const std::string& separator, vector<std::string>& result);

	string PlatformMessage(const MovingPlatform * platform);

	string ClientMessage(const string& name, const Character * character);
public:
	Server();

	void receiverHandler(GameTime* gameTime);

	void publisherHandler(list<MovingPlatform*>* platforms);
};


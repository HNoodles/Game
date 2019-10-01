#pragma once
#include <zmq.hpp>
#include <string>
#include <iostream>
#include "../Objects/Character.h"

constexpr auto CLIENT_NAME = "A";

using namespace std;
using namespace zmq;

class Client
{
private:
	context_t context;
	socket_t sender;
	socket_t subscriber;
	map<string, Character*>* characters;

	string s_recv(socket_t& socket);

	void s_send(socket_t& socket, const string& string);

	void Split(const string& string, const std::string& separator, vector<std::string>& result);

	string ClientMessage(const string& name, const Character* character);

public:
	Client(map<string, Character*>* characters);

	void sendHandler();

	void subscribeHandler(list<MovingPlatform*>* platforms);
};


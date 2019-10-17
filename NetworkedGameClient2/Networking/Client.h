#pragma once
#include <zmq.hpp>
#include <string>
#include <iostream>
#include "../Objects/Character.h"

constexpr auto CLIENT_NAME = "B";

using namespace std;
using namespace zmq;

class Client
{
private:
	context_t context;
	socket_t sender;
	socket_t subscriber;
	map<string, Vector2f>* characters;
	Character* thisChar;
	bool connected;

	string s_recv(socket_t& socket);

	void s_send(socket_t& socket, const string& string);

	void Split(const string& string, const std::string& separator, vector<std::string>& result);

	string ClientMessage(const string& name, Character* character);

public:
	Client(Character* thisChar, map<string, Vector2f>* characters);

	void sendHandler();

	void subscribeHandler(list<Collidable*>* collidableObjects);

	void disconnect();
};


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
#include "../Objects/SpawnPoint.h"
#include "../Objects/DeathZone.h"

using namespace std;
using namespace zmq;

constexpr auto SERVER_NAME = "S";

class Server
{
private:
	context_t context;
	socket_t receiver;
	socket_t publisher;
	map<string, Character*> characters;

	string s_recv(socket_t& socket);

	void s_send(socket_t& socket, const string& string);

	void Split(const string& string, const std::string& separator, vector<std::string>& result);

	string CollidableObjectMessage(Collidable * object);

	string PlatformMessage(MovingPlatform* platform);

	string SpawnPointMessage(SpawnPoint* spawnPoint);

	string DeathZoneMessage(DeathZone* deathZone);

	string CharacterMessage(const string& name, Character* character);

	void disconnectHandler(const string& name);
public:
	Server();

	~Server();

	void receiverHandler(GameTime* gameTime);

	void publisherHandler(list<Collidable*>* collidableObjects);
};


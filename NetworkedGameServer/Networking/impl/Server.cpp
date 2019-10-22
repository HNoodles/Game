#include "../Server.h"

Server::Server()
	: context(1), receiver(context, ZMQ_REP), publisher(context, ZMQ_PUB)
{
	receiver.bind("tcp://*:5555");
	cout << "Receiver started, listening for clients on port 5555..." << endl;

	publisher.bind("tcp://*:5556");
	cout << "Publisher started, publishing messages on port 5556..." << endl;
}

Server::~Server()
{
	// release all characters generated
	for (auto pair : characters)
		delete pair.second;
}

void Server::receiverHandler(GameTime* gameTime)
{
	while (true)
	{
		// listen from clients
		string client_string = s_recv(receiver); // name x y(position of character of client) (time_step)
		vector<string> result;

		// split string
		Split(client_string, " ", result);

		// check if disconnecting message: name D
		if (result.size() == 2)
		{
			disconnectHandler(result[0]);
			// go to next loop
			continue;
		}

		// find client
		auto iter = characters.find(result[0]);
		// store client into map
		if (iter == characters.end()) // new client, generate object
		{
			LocalTime local(1, *gameTime);

			characters.insert({ 
				result[0], 
				new Character(
					::Shape::DIAMOND, ::Color::BLUE, Vector2f(60.f, 120.f),
					Vector2f((float)atof(result[1].c_str()), (float)atof(result[2].c_str())), // pos
					Vector2f(250.0f, 0.0f), local
				)
			});

			cout << "New client " + result[0] << endl;
		}
		else // old client, set position
		{
			Character* character = iter->second;
			dynamic_cast<Renderable*>(character->getGC(ComponentType::RENDERABLE))->getShape()->setPosition(
				Vector2f((float)atof(result[1].c_str()), (float)atof(result[2].c_str()))
			);
		}
		
		// send a response to fulfill a come and go
		s_send(receiver, "success");
	}
}

void Server::publisherHandler(list<Collidable*>* collidableObjects)
{	
	// publish current message
	string message = "";

	// generate platforms message
	for (Collidable* object : *collidableObjects)
	{
		message += CollidableObjectMessage(object);
	}

	string deleted = "";
	// generate clients message
	for (auto pair : characters)
	{
		message += CharacterMessage(pair.first, pair.second);

		if (!pair.second) // disconnected client, delete it
		{
			//characters.erase(pair.first);
			deleted = pair.first;
		}
	}
	// delete pair of disconnected client after notifying everyone
	if (characters.find(deleted) != characters.end())
	{
		characters.erase(deleted);
	}

	// send message
	s_send(publisher, message);

	// sleep for 16ms to avoid too frequent publish
	Sleep(16);
}

void Server::disconnectHandler(const string& name)
{
	// find and release the character pointer, 
	// reset the second value to nullptr
	auto pair = characters.find(name);
	delete pair->second;
	pair->second = NULL;

	// send response back to client
	s_send(receiver, "Disconnected");

	cout << "Client " + name + " disconnected" << endl;
}

string Server::s_recv(socket_t& socket)
{
	message_t message;
	socket.recv(message);

	return string(static_cast<char*>(message.data()), message.size());
}

void Server::s_send(socket_t& socket, const string& string)
{
	message_t message(string.size());
	memcpy(message.data(), string.data(), string.size());

	socket.send(message, send_flags::none);
}

void Server::Split(const string& string, const std::string& separator, vector<std::string>& result)
{
	string::size_type start = 0, index = 0;
	result.clear();

	while (index != string::npos)
	{
		index = string.find_first_of(separator, start);
		result.emplace_back(string.substr(start, index - start));
		start = index + strlen(separator.c_str());
	}
}

string Server::CollidableObjectMessage(Collidable * object)
{
	// TYPE 5.0 5.0\n
	Collision type = object->getType();
	switch (type)
	{
	case Collision::PLATFORM: 
		return PlatformMessage(dynamic_cast<MovingPlatform*>(object->getGameObject()));
	case Collision::DEATHZONE:
		return DeathZoneMessage(dynamic_cast<DeathZone*>(object->getGameObject()));
	case Collision::CHARACTER: // won't have this type
	case Collision::SIDEBOUNDARY: // won't have this type
	default: // shouldn't reach here
		return "";
	}
}

string Server::PlatformMessage(MovingPlatform* platform) // P 5.0 5.0 0\n
{
	string message = "P ";

	string heading = " " + to_string(platform->getHeadingPositive());
	Vector2f pos = dynamic_cast<Renderable*>(platform->getGC(ComponentType::RENDERABLE))
		->getShape()->getPosition();

	message += to_string(pos.x) + " " + to_string(pos.y) + heading + "\n";

	return message;
}

string Server::SpawnPointMessage(SpawnPoint* spawnPoint) // S 5.0 5.0\n
{
	string message = "S ";

	Vector2f pos = dynamic_cast<Renderable*>(spawnPoint->getGC(ComponentType::RENDERABLE))
		->getShape()->getPosition();

	message += to_string(pos.x) + " " + to_string(pos.y) + "\n";

	return message;
}

string Server::DeathZoneMessage(DeathZone* deathZone) // D pos.x pos.y\n
{
	string message = "D ";

	sf::Shape* shape = dynamic_cast<Renderable*>(deathZone->getGC(ComponentType::RENDERABLE))->getShape();
	Vector2f pos = shape->getPosition();

	message += to_string(pos.x) + " " + to_string(pos.y) + "\n";

	return message;
}

string Server::CharacterMessage(const string& name, Character* character)
{
	if (!character) // already disconnected character, notify all clients: C name D
	{
		return "C " + name + " D\n";
	}
	else // C NAME 5.0 5.0 1.234\n
	{
		Vector2f pos = dynamic_cast<Renderable*>(character->getGC(ComponentType::RENDERABLE))
			->getShape()->getPosition();
		Timeline& time = dynamic_cast<Movable*>(character->getGC(ComponentType::MOVABLE))->getTimeline();
		return "C " + name + " " + to_string(pos.x) + " " + to_string(pos.y) + " " 
			+ to_string(time.getTime()) + "\n";
	}
}
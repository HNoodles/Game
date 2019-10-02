#include "Server.h"

Server::Server()
	: context(1), receiver(context, ZMQ_REP), publisher(context, ZMQ_PUB)
{
	receiver.bind("tcp://*:5555");
	cout << "Receiver started, listening for clients on port 5555..." << endl;

	publisher.bind("tcp://*:5556");
	cout << "Publisher started, publishing messages on port 5556..." << endl;
}

void Server::receiverHandler(GameTime* gameTime)
{
	while (true)
	{
		// listen from clients
		string client_string = s_recv(receiver); // name, x, y(position of character of client), (time_step)
		vector<string> result;

		//cout << client_string << endl;

		// split string
		Split(client_string, " ", result);

		// find client
		auto iter = characters.find(result[0]);
		// store client into map
		if (iter == characters.end()) // new client
		{
			LocalTime local(1, *gameTime);
			Character character(Vector2f(250.0f, 0.0f), local);
			character.setPosition(Vector2f((float)atof(result[1].c_str()), (float)atof(result[2].c_str())));
			characters.insert({ result[0], character });
			
			cout << "New client " + result[0] << endl;
		}
		else // old client
		{
			Character& character = iter->second;
			character.setPosition(Vector2f((float)atof(result[1].c_str()), (float)atof(result[2].c_str())));
			//characters->emplace(result[0], character);

			//if (result.size() > 3) // has set time step
			//{
			//	client.time.setStepSize(atof(result[4].c_str()));
			//}
			
			//cout << "Old client " + result[0] << endl;
		}
		
		// send a response to fulfill a come and go
		s_send(receiver, "success");
	}
}

void Server::publisherHandler(list<MovingPlatform*>* platforms)
{	
	// publish current message
	string message = "";

	// generate platforms message
	for (MovingPlatform* platform : *platforms)
	{
		message += PlatformMessage(platform);
	}

	// generate clients message
	for (auto pair : characters)
	{
		message += ClientMessage(pair.first, &pair.second);
	}

	// send message
	s_send(publisher, message);

	// sleep for 16ms to avoid too frequent publish
	Sleep(16);
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

string Server::PlatformMessage(const MovingPlatform * platform)
{
	// P 5.0 5.0 1\n
	Vector2f pos = platform->getPosition();
	return "P " + to_string(pos.x) + " " + to_string(pos.y) + " " 
		+ to_string(platform->getHeadingPositive()) + "\n";
}

string Server::ClientMessage(const string& name, const Character * character)
{
	// C A 5.0 5.0 1.234\n
	Vector2f pos = character->getPosition();
	return "C " + name + " " + to_string(pos.x) + " " + to_string(pos.y) + " " 
		+ to_string(character->getTimeline().getTime()) + "\n";
}
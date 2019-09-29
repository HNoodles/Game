#include "Server.h"

Server::Server(list<MovingPlatform*>* platforms) 
	: context(1), receiver(context, ZMQ_REP), publisher(context, ZMQ_PUB)
{
	receiver.bind("tcp://*:5555");
	cout << "Receiver started, listening for clients on port 5555..." << endl;

	publisher.bind("tcp://*:5556");
	cout << "Publisher started, publishing messages on port 5556..." << endl;
}

void Server::receiverHandler(GameTime* gameTime, map<string, Character*>* characters)
{
	while (true)
	{
		// listen from clients
		string client_string = s_recv(receiver); // name, x, y(position of character of client), (time_step)
		vector<string> result;

		// split string
		Split(client_string, " ", result);

		// find client
		auto iter = clients.find(result[0]);
		// store client into map
		if (iter == clients.end()) // new client
		{
			Client client = { LocalTime(1, *gameTime), (float)atof(result[1].c_str()), (float)atof(result[2].c_str()) };
			clients.emplace(result[0], client);
			cout << "New client " + result[0] << endl;
		}
		else // old client
		{
			Client client = { iter->second.time, (float)atof(result[1].c_str()), (float)atof(result[2].c_str()) };
			if (result.size() > 3) // has set time step
			{
				client.time.setStepSize(atof(result[4].c_str()));
			}
			clients.emplace(result[0], client);
			//cout << "Old client " + result[0] << endl;
		}

		// generate character object according to latest client info
		Client client = clients.find(result[0])->second;
		Character character(Vector2f(250.0f, 0.0f), *gameTime);
		character.setPosition(Vector2f(client.x, client.y));
		characters->emplace(result[0], &character);
		
		// send a response to fulfill a come and go
		s_send(receiver, "success");
	}
}

void Server::publisherHandler(list<MovingPlatform*>* platforms)
{
	// keep publishing current state of connection
	//while (true)
	//{
		// publish current message
		string message = "";

		// generate platforms message
		for (MovingPlatform* platform : *platforms)
		{
			Vector2f pos = platform->getPosition();
			message += PlatformMessage(pos.x, pos.y);
		}

		// generate clients message
		for (auto client = clients.begin(); client != clients.end(); client++)
		{
			message += ClientMessage(client->first, client->second.x, client->second.y, client->second.time.getTime());
		}

		// send message
		s_send(publisher, message);
		//cout << message << endl;
	//}
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
	string::size_type start = 0, index;
	result.clear();

	while (start != string::npos)
	{
		index = string.find_first_of(separator, start);
		result.emplace_back(string.substr(start, index - start));
		start = index + strlen(separator.c_str());
	}
}

string Server::PlatformMessage(float x, float y)
{
	// 5.0 5.0\n
	return "P " + to_string(x) + " " + to_string(y) + "\n";
}

string Server::ClientMessage(string name, float x, float y, double thisTime)
{
	// A 5.0 5.0\n
	return "C " + name + " " + to_string(x) + " " + to_string(y) + " " + to_string(thisTime) + "\n";
}
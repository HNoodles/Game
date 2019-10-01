#include "Client.h"

Client::Client(map<string, Character*>* characters)
	: context(1), sender(context, ZMQ_REQ), subscriber(context, ZMQ_SUB), characters(characters)
{
	sender.connect("tcp://localhost:5555");
	cout << "Connecting to server on port 5555..." << endl;

	subscriber.connect("tcp://localhost:5556");
	subscriber.setsockopt(ZMQ_SUBSCRIBE, "P", strlen("P"));
	cout << "Subscribing to server on port 5556..." << endl;
}

void Client::sendHandler()
{
	// find the character of this client
	Character* thisChar = characters->find(CLIENT_NAME)->second;

	// generate message accordingly
	string message = ClientMessage(CLIENT_NAME, thisChar);

	// send message and receive response
	s_send(sender, message);
	string response = s_recv(sender);
}

void Client::subscribeHandler(list<MovingPlatform*>* platforms)
{
	while (true)
	{
		string message = s_recv(subscriber);

		// prepare iterator
		auto iterP = platforms->begin();

		// split into lines
		vector<string> lines;
		Split(message, "\n", lines);

		for (const string& line : lines)
		{
			// split into parts of information
			vector<string> infos;
			Split(line, " ", infos);

			// analyze infos and relocate objects
			if (infos[0] == "P") // platform info
			{
				// get position info
				Vector2f pos((float)atof(infos[1].c_str()), (float)atof(infos[2].c_str()));
				// set position
				(*iterP)->setPosition(pos);
				// set head positive
				(*iterP)->setHeadingPositive(infos[3] == "1");
				// move to next iterP
				iterP++;
			}
			else if (infos[0] == "C") // client info
			{
				// get position info
				Vector2f pos((float)atof(infos[2].c_str()), (float)atof(infos[3].c_str()));
				// get game time
				GameTime time(1, atof(infos[4].c_str()));

				// get character by name
				auto pair = characters->find(infos[1]);

				if (pair != characters->end()) // old character
				{
					// find old character instance
					Character* c = pair->second;
					// update character info
					//c->setPosition(pos);
					c->setTimeline(time);
				}
				else // new character
				{
					// create new character instance
					Character c(Vector2f(250.0f, 0.0f), time);
					c.setPosition(pos);
					// add to map
					characters->emplace(infos[1], &c);
				}
			}
		}

	}
}

string Client::s_recv(socket_t& socket)
{
	message_t message;
	socket.recv(message);

	return string(static_cast<char*>(message.data()), message.size());
}

void Client::s_send(socket_t& socket, const string& string)
{
	message_t message(string.size());
	memcpy(message.data(), string.data(), string.size());

	socket.send(message, send_flags::none);
}

void Client::Split(const string& string, const std::string& separator, vector<std::string>& result)
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

string Client::ClientMessage(const string& name, const Character* character)
{
	// A 5.0 5.0\n
	Vector2f pos = character->getPosition();
	return name + " " + to_string(pos.x) + " " + to_string(pos.y) + "\n";
}

#include "../Client.h"

Client::Client(Character* thisChar, map<string, Vector2f>* characters)
	: context(1), sender(context, ZMQ_REQ), subscriber(context, ZMQ_SUB), 
	thisChar(thisChar), characters(characters), connected(true)
{
	sender.connect("tcp://localhost:5555");
	cout << "Connecting to server on port 5555..." << endl;

	subscriber.connect("tcp://localhost:5556");
	subscriber.setsockopt(ZMQ_SUBSCRIBE, "P", strlen("P"));
	cout << "Subscribing to server on port 5556..." << endl;
}

void Client::sendHandler()
{
	if (!connected) // don't send if disconnected
		return;

	// generate message accordingly
	string message = ClientMessage(CLIENT_NAME, thisChar);

	// send message and receive response
	s_send(sender, message);
	string response = s_recv(sender);
}

void Client::subscribeHandler(list<Collidable*>* collidableObjects)
{
	while (true)
	{
		string message = s_recv(subscriber);

		// prepare iterator
		auto iterP = collidableObjects->begin();

		// split into lines
		vector<string> lines;
		Split(message, "\n", lines);

		for (const string& line : lines)
		{
			// split into parts of information
			vector<string> infos;
			Split(line, " ", infos);

			// analyze infos and relocate objects
			if (infos[0] == "P") // platform info, P 5.0 5.0\n
			{
				// get position info
				Vector2f pos((float)atof(infos[1].c_str()), (float)atof(infos[2].c_str()));
				// set position
				(*iterP)->getRenderable()->getShape()->setPosition(pos);
				// set head positive
				(*iterP)->getMovable()->setHeadingPositive(infos[3] == "1");
				// move to next iterP
				iterP++;
			}
			else if (infos[0] == "C") // client info, C NAME 5.0 5.0 1.234\n
			{
				if (infos[2] == "D") // C name D, client disconnected
				{
					// delete stored character pair
					characters->erase(infos[1]);
					continue;
				}

				// get position info
				Vector2f pos((float)atof(infos[2].c_str()), (float)atof(infos[3].c_str()));
				
				if (infos[1] == CLIENT_NAME) // this client, update pos and time
				{
					// get game time
					GameTime time(1, atof(infos[4].c_str()));

					dynamic_cast<Renderable*>(thisChar->getGC(ComponentType::RENDERABLE))
						->getShape()->setPosition(pos);
					dynamic_cast<Movable*>(thisChar->getGC(ComponentType::MOVABLE))
						->setTimeline(time);
				}
				else // other client, only update pos
				{
					(*characters)[infos[1]] = pos;
				}
			}
		}

	}
}

void Client::disconnect()
{
	// prevent from sending messages out
	connected = false;

	// generate message accordingly
	string message = (string)CLIENT_NAME + " D";

	// send message and receive response
	s_send(sender, message);
	string response = s_recv(sender);
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

string Client::ClientMessage(const string& name, Character* character)
{
	// A 5.0 5.0\n
	Vector2f pos = dynamic_cast<Renderable*>(character->getGC(ComponentType::RENDERABLE))
		->getShape()->getPosition();
	return name + " " + to_string(pos.x) + " " + to_string(pos.y) + "\n";
}

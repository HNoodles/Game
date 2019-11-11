#include "../Client.h"

Client::Client(map<string, GameObject*>* objects, EventManager* manager)
	: context(1), sender(context, ZMQ_REQ), subscriber(context, ZMQ_SUB), 
	objects(objects), connected(true), connectedTime(0), manager(manager)
{
	sender.connect("tcp://localhost:5555");
	cout << "Connecting to server on port 5555..." << endl;

	subscriber.connect("tcp://localhost:5556");
	subscriber.setsockopt(ZMQ_SUBSCRIBE, "GVT", strlen("GVT"));
	cout << "Subscribing to server on port 5556..." << endl;
}

void Client::connect()
{
	// send self name to server to connect
	s_send(sender, SELF_NAME);

	// receive and set connected time
	string response = s_recv(sender);
	connectedTime = atof(response.c_str());

	cout << "Connected to server" << endl;
}

void Client::sendHandler()
{
	if (!connected) // don't send if disconnected
		return;
	
	// first, name GVT double 
	string message = SELF_NAME + (string)" GVT " + to_string(manager->getRequestGVT()) + "\n";

	// generate events string, SELF_NAME E executeTime ObjID X_val Y_val
	list<EObjMovement>* newObjMovements = manager->getObjMovements();
	manager->getMtxEvt()->lock();
	for (EObjMovement e : *newObjMovements)
	{
		// only send this character movement event
		if (e.getObject()->getId() == SELF_NAME)
			message += SELF_NAME + (string)" " + e.toString();
	}
	newObjMovements->clear();
	manager->getMtxEvt()->unlock();

	//cout << message << endl;

	// send message and receive response
	s_send(sender, message);
	string response = s_recv(sender);
}

void Client::subscribeHandler(GameTime* gameTime)
{
	while (true)
	{
		string message = s_recv(subscriber);

		// split into lines
		vector<string> lines;
		Split(message, "\n", lines);

		//cout << message << endl;

		// first line is GVT
		vector<string> infos;
		Split(lines[0], " ", infos);
		manager->setGVT(atof(infos[1].c_str()) - connectedTime);
		lines.erase(lines.begin());

		for (const string& line : lines)
		{
			// skip empty lines
			if (line == "")
				continue;

			// split into parts of information
			vector<string> infos;
			Split(line, " ", infos);

			if (infos[0] == "C" && infos[2] == "D") // C name D, client disconnected
			{
				manager->removeQueue((const char*)infos[1][0]);
				manager->removeGVT((const char*)infos[1][0]);
				objects->erase(infos[1]);
				continue;
			}

			// SELF_NAME E executeTime ObjID X_val Y_val Positive
			if (infos[3] == SELF_NAME) // skips events of self
				continue;
			
			// new object if is new object (newly connected client)
			auto iter = objects->find(infos[3]);
			// store client into map
			if (iter == objects->end()) // new client, generate object
			{
				LocalTime local(1, *gameTime);

				objects->insert({
					infos[3],
					new Character(
						infos[3], manager,
						::Shape::DIAMOND, ::Color::BLUE, Vector2f(60.f, 120.f),
						Vector2f((float)atof(infos[4].c_str()), (float)atof(infos[5].c_str())), // pos
						Vector2f(250.0f, 0.0f), local
					)
				});

				cout << "New client " + infos[0] << endl;
			}

			// insert new Event anyway
			manager->getMtxQueue()->lock();
			manager->insertEvent(
				new EObjMovement(
					atof(infos[2].c_str()) - connectedTime, // add time bias
					objects->find(infos[3])->second,// character
					atof(infos[4].c_str()),
					atof(infos[5].c_str()), 
					infos[6] == "1"
				),
				(const char* const)infos[0][0]
			);
			manager->getMtxQueue()->unlock();
		}
	}
}

void Client::disconnect()
{
	// prevent from sending messages out
	connected = false;

	// generate message accordingly
	string message = (string)SELF_NAME + " D";

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

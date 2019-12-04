#include "../Client.h"
#include "../../Objects/Invader.h"

Client::Client(map<string, GameObject*>* objects, EventManager* manager)
	: context(1), sender(context, ZMQ_REQ), subscriber(context, ZMQ_SUB), 
	objects(objects), connected(false), connectedTime(0), manager(manager), win(false)
{
	sender.connect("tcp://localhost:5555");
	cout << "Connecting to server on port 5555..." << endl;

	subscriber.connect("tcp://localhost:5556");
	subscriber.setsockopt(ZMQ_SUBSCRIBE, "GVT", strlen("GVT"));
	cout << "Subscribing to server on port 5556..." << endl;
}

Client::~Client()
{
	// clear generated objects
	for (auto& pair : *objects)
	{
		delete pair.second;
	}
	// clear expired objects
	for (GameObject* object : expired)
	{
		delete object;
	}
}

void Client::connect(Character* character)
{
	// try to connect to server
	Vector2f pos = dynamic_cast<Renderable*>(character->getGC(ComponentType::RENDERABLE))
		->getShape()->getPosition();

	// SELF_NAME time pos_x pos_y
	s_send(
		sender, 
		SELF_NAME + (string)" " + to_string(manager->getCurrentTime()) + " " + 
		to_string(pos.x) + " " + to_string(pos.y)
	);

	// receive and set connected time
	string response = s_recv(sender);
	connectedTime = atof(response.c_str());

	connected = true;

	cout << "Connected to server" << endl;
}

void Client::sendHandler(Character* character)
{
	if (!connected) // don't send if disconnected
		return;
	
	// first, name GVT double 
	string message = SELF_NAME + (string)" GVT " + to_string(manager->getRequestGVT() + connectedTime) + "\n";

	// generate expired bullets message, BulletID = SELFNAME + ROUNDNUM
	list<Bullet*>* expired = character->getExpiredBullets();
	for (Bullet* bullet : *expired)
	{
		message += bullet->getId() + "\n";
		delete bullet;
	}
	expired->clear();

	// generate events string, SELF_NAME E executeTime ObjID X_val Y_val
	list<EObjMovement>* newObjMovements = manager->getObjMovements();
	manager->getMtxEvt()->lock();
	for (EObjMovement e : *newObjMovements)
	{
		string id = e.getObject()->getId();
		// only send this character movement event and character's bullets movement
		if (id.find(SELF_NAME) == 0)
			message += id + (string)" " + e.toString();
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
	while (connected)
	{
		string message = s_recv(subscriber);

		// split into lines
		vector<string> lines;
		Split(message, "\n", lines);

		//cout << message << endl;

		if (lines[0] == "GVTWIN") // win
		{
			win = true;
			break;
		}

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

			// ObjID, move expired object to expired list
			if (infos.size() == 1)
			{
				expired.push_back((*objects)[infos[0]]);
				objects->erase(infos[0]);
				continue;
			}

			// SELF_NAME E executeTime ObjID X_val Y_val Positive
			//if (infos[3].find(SELF_NAME) == 0) // skips events of self
			//	continue;
			
			if (objects->count(infos[3]) == 0) // new game object
			{
				if (infos[3].find("I") == 0) // new invader
				{
					objects->insert({
						infos[3], 
						new Invader(
							infos[3], manager, Vector2f((float)atof(infos[4].c_str()), (float)atof(infos[5].c_str())),
							Vector2f(50.f, 50.f), *gameTime, nullptr
						)
					});
				}
				else if (infos[3].find("B") == 0) // new invader bullet
				{
					objects->insert({
						infos[3],
						new Bullet(
							infos[3], manager, Vector2f((float)atof(infos[4].c_str()), (float)atof(infos[5].c_str())),
							*gameTime, true
						)
					});
				}
			}
			
			// insert new Event if object still exists
			lock_guard<mutex> guard(*manager->getMtxQueue());
			if (objects->count(infos[3]) == 0)
				continue;
			manager->insertEvent(
				new EObjMovement(
					atof(infos[2].c_str()) - connectedTime, // add time bias
					objects->find(infos[3])->second,// object
					atof(infos[4].c_str()),
					atof(infos[5].c_str()), 
					infos[6] == "1"
				),
				(const char* const)infos[0][0]
			);
		}
	}
}

void Client::disconnect(bool forReplay)
{
	// prevent from sending messages out and receiving messages
	connected = false;
	// set manager to disconnected
	// this will clear all events in manager queues
	lock_guard<mutex> guard(*manager->getMtxQueue());
	manager->setConnected(false);

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

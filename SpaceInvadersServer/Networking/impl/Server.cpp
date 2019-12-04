#include "../Server.h"

Server::Server(map<string, GameObject*>* objects, EventManager* manager)
	: context(1), receiver(context, ZMQ_REP), publisher(context, ZMQ_PUB), objects(objects), manager(manager)
{
	receiver.bind("tcp://*:5555");
	cout << "Receiver started, listening for clients on port 5555..." << endl;

	publisher.bind("tcp://*:5556");
	cout << "Publisher started, publishing messages on port 5556..." << endl;
}

Server::~Server()
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

void Server::receiverHandler(GameTime* gameTime)
{
	while (true)
	{
		// listen from clients
		string client_string = s_recv(receiver); 

		//cout << client_string << endl;

		// check if special cases 
		vector<string> result;
		Split(client_string, " ", result);

		// disconnecting message: name D
		if (result.size() == 2 && result[1] == "D")
		{
			disconnectHandler(result[0]);
			// go to next loop
			continue;
		}

		// connecting message: name time pos_x pos_y
		if (result.size() == 4 && result[1] != "D")
		{
			connectHandler(result[0], atof(result[1].c_str()));
			// generate character object
			objects->insert({
				result[0], 
				new Character(
					result[0], manager,
					::Shape::DIAMOND, ::Color::BLUE, Vector2f(30.f, 60.f), 
					Vector2f((float)atof(result[2].c_str()), (float)atof(result[3].c_str())),
					Vector2f(100.0f, 0.0f), *gameTime 
				)
			});
			// go to next loop
			break;
		}

		// split into lines
		vector<string> lines;
		Split(client_string, "\n", lines);

		// name GVT double
		Split(lines[0], " ", result);
		manager->insertGVT((const char* const)result[0][0], atof(result[2].c_str()));
		lines.erase(lines.begin()); // remove first line

		for (string line : lines)
		{
			// skip empty lines
			if (line == "")
				continue;

			Split(line, " ", result);

			// BulletID, expired character's bullet
			if (result.size() == 1)
			{
				expired.push_back((*objects)[result[0]]);
				dynamic_cast<Character*>((*objects)["A"])->removeBullet(
					dynamic_cast<Bullet*>((*objects)[result[0]])
				);
				objects->erase(result[0]);
				continue;
			}

			// SELF_NAME E executeTime ObjID X_val Y_val
			// we only care about character infos and its bullets' infos, whose ObjID begins with A
			if (result[3].find("A") != 0)
				continue;

			// new object, store client into map
			if (objects->count(result[3]) == 0) // new object, generate object
			{
				if (result[3].find("A") == 0) // new character bullet, insert into character bullet list
				{
					objects->insert({
						result[3],
						new Bullet(
							result[3], manager, Vector2f((float)atof(result[4].c_str()), (float)atof(result[5].c_str())),
							*gameTime, false
						)
					});
					dynamic_cast<Character*>((*objects)["A"])->insertBullet(
						dynamic_cast<Bullet*>((*objects)[result[3]])
					);
				}
			}
			
			// insert new Event anyway
			manager->getMtxQueue()->lock();
			manager->insertEvent(
				new EObjMovement(
					atof(result[2].c_str()) + connectTimes[result[0]], // add bias time
					objects->find(result[3])->second,// object
					atof(result[4].c_str()),
					atof(result[5].c_str())
				), 
				(const char* const)result[0][0]
			);
			manager->getMtxQueue()->unlock();
		}
		
		// send a response to fulfill a come and go
		s_send(receiver, "success");
	}
}

void Server::publisherHandler(InvaderMatrix* invaders)
{		
	// check if is win
	if (invaders->getWin())
	{
		s_send(publisher, "GVTWIN");
		return;
	}

	// start with requested GVT
	string message = "GVT " + to_string(manager->getGVT()) + "\n";

	// generate expired bullets message, BulletID = B + invaderID + roundnum
	list<Bullet*>* expired = invaders->getExpiredBullets();
	for (Bullet* bullet : *expired)
	{
		message += bullet->getId() + "\n";
		//delete bullet;
		this->expired.push_back(bullet);
	}
	expired->clear();

	// generate killed invader message, InvaderID = I + row + column
	list<Invader*>* killed = invaders->getKilled();
	for (Invader* invader : *killed)
	{
		message += invader->getId() + "\n";
		//delete invader;
		this->expired.push_back(invader);
	}
	killed->clear();

	// generate events string, SELF_NAME E executeTime ObjID X_val Y_val
	list<EObjMovement>* newObjMovements = manager->getObjMovements();
	manager->getMtxEvt()->lock();
	for (EObjMovement e : *newObjMovements)
	{
		message += SELF_NAME + (string)" " + e.toString();
	}
	newObjMovements->clear();
	manager->getMtxEvt()->unlock();

	//cout << message << endl;

	// send message
	s_send(publisher, message);
}

void Server::connectHandler(const string& name, double time)
{
	// get current time
	double connectTime = manager->getCurrentTime();

	// store connect time
	connectTimes.insert({ name, connectTime - time });

	// send connect time back
	s_send(receiver, to_string(connectTimes[name]));

	cout << "New client " + name << endl;
}

void Server::disconnectHandler(const string& name)
{
	// remove the queue of the client, remove GVT of the client
	manager->getMtxQueue()->lock();
	manager->removeQueue((const char* const)name[0]);
	manager->removeGVT((const char* const)name[0]);
	list<EObjMovement>* objMovements = manager->getObjMovements();
	manager->getMtxEvt()->lock();
	for (auto iter = objMovements->begin(); iter != objMovements->end(); )
	{
		// nothing in list
		if (iter == objMovements->end())
		{
			break;
		}
		// remove movements of disconnected character
		if (iter->getObject()->getId() == name[0])
		{
			objMovements->erase(iter++);
		}
		else
		{
			iter++;
		}
	}
	manager->getMtxEvt()->unlock();
	manager->getMtxQueue()->unlock();
	//// remove the character pointer in characters
	//delete (*objects)[name];
	//objects->erase(name);
	// remove the connect time
	connectTimes.erase(name);

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


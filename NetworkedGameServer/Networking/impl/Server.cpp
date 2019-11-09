#include "../Server.h"

Server::Server(EventManager* manager)
	: context(1), receiver(context, ZMQ_REP), publisher(context, ZMQ_PUB), manager(manager), mtxEvt(manager->getMtxEvt())
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
		string client_string = s_recv(receiver); 

		// check if disconnecting message: name D
		vector<string> result;
		Split(client_string, " ", result);
		if (result.size() == 2)
		{
			disconnectHandler(result[0]);
			// go to next loop
			continue;
		}

		// split into lines
		vector<string> lines;
		Split(client_string, "\n", lines);

		// name GVT double
		Split(lines[0], " ", result);
		manager->insertGVT((const char* const)result[0][0], atof(result[2].c_str()));
		lines.erase(lines.begin()); // remove first line

		// SELF_NAME E executeTime ObjID(client_name) X_val Y_val
		for (string line : lines)
		{
			Split(line, " ", result);

			// we only care about character infos, whose ObjID has length of 1
			if (result[3].length() > 1)
				continue;

			// new character if is newly connected client
			auto iter = characters.find(result[3]);
			// store client into map
			if (iter == characters.end()) // new client, generate object
			{
				LocalTime local(1, *gameTime);

				characters.insert({
					result[3],
					new Character(
						result[3], manager, 
						::Shape::DIAMOND, ::Color::BLUE, Vector2f(60.f, 120.f),
						Vector2f((float)atof(result[4].c_str()), (float)atof(result[5].c_str())), // pos
						Vector2f(250.0f, 0.0f), local
					)
				});

				cout << "New client " + result[0] << endl;
			}
			
			// insert new Event anyway
			manager->insertEvent(
				new EObjMovement(
					atof(result[2].c_str()),
					characters.find(result[3])->second,// character
					atof(result[4].c_str()),
					atof(result[5].c_str())
				), 
				(const char* const)result[0][0]
			);
		}
		
		// send a response to fulfill a come and go
		s_send(receiver, "success");

		// update GVT and execute events
		manager->updateGVT();
		manager->executeEvents();
	}
}

void Server::publisherHandler()
{	
	// start with requested GVT
	string message = "GVT " + to_string(manager->getGVT()) + "\n";

	// generate disconnection messages
	mtxDisc.lock();
	for (string name : disconnecting)
	{
		message += "C " + name + " D\n";
	}
	// empty the list afterwards
	disconnecting.empty();
	mtxDisc.unlock();

	// generate events string, SELF_NAME E executeTime ObjID X_val Y_val
	list<EObjMovement>* newObjMovements = manager->getObjMovements();
	mtxEvt->lock();
	for (EObjMovement e : *newObjMovements)
	{
		message += SELF_NAME + (string)" " + e.toString();
	}
	newObjMovements->clear();
	mtxEvt->unlock();

	// send message
	s_send(publisher, message);

	// sleep for 16ms to avoid too frequent publish
	Sleep(16);
}

void Server::disconnectHandler(const string& name)
{
	// remove the queue of the client, remove GVT of the client
	manager->removeQueue((const char* const)name[0]);
	manager->removeGVT((const char* const)name[0]);
	// remove the character pointer in characters
	delete characters.find(name)->second;
	characters.erase(name);

	// store the disconnected client in the list
	mtxDisc.lock();
	disconnecting.emplace_back(name);
	mtxDisc.unlock();

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
//
//  Hello World server in C++
//  Binds REP socket to tcp://*:5555
//  Expects "Hello" from client, replies with "World"
//
#include <zmq.hpp>
#include <string>
#include <iostream>
#ifndef _WIN32
#include <unistd.h>
#else
#include <windows.h>

#define sleep(n)    Sleep(n)
#endif

using namespace std;
using namespace zmq;


typedef struct {
	string name;
	int iteration;
} Client;

string s_recv(socket_t& socket);
void s_send(socket_t& socket, const string& string);
vector<Client>::iterator l_find(vector<Client>& clients, string name);
string Message(int num, int iter);

int main() 
{
	//  Prepare our context and socket
	context_t context(1);
	socket_t socket(context, ZMQ_REP);
	socket.bind("tcp://*:5555");

	vector<Client> clients;

	while (true)
	{
		// listen from clients
		string client_name = s_recv(socket);

		// find the client stored
		auto iter = l_find(clients, client_name);
		if (iter == clients.end())
		{// new client
			Client client = {client_name, 1};
			clients.emplace_back(client);
			cout << "New client " + client_name << endl;
		}
		else
		{// old client, update iteration count
			iter->iteration++;
			cout << "Old client " + client_name << endl;
		}

		// send messages
		string message = "";
		for (int i = 0; i < clients.size(); i++)
		{
			message += Message(i + 1, clients[i].iteration);
		}
		s_send(socket, message);
	}

	return 0;
}

//  Receive 0MQ string from socket and convert into string
static string s_recv(socket_t& socket)
{
	message_t message;
	socket.recv(message);

	return string(static_cast<char*>(message.data()), message.size());
}

//  Convert string to 0MQ string and send to socket
static void s_send(socket_t& socket, const string& string)
{
	message_t message(string.size());
	memcpy(message.data(), string.data(), string.size());

	socket.send(message, send_flags::dontwait);
}

vector<Client>::iterator l_find(vector<Client>& clients, string name)
{
	auto iter = clients.begin();
	for (; iter != clients.end(); iter++)
	{
		if (iter->name == name)
		{
			return iter;
		}
	}
	return iter;
}

string Message(int num, int iter) 
{ 
	return "Client " + to_string(num) + ": Iteration " + to_string(iter) + "\n";
}
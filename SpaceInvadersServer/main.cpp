#include <SFML/Graphics.hpp>
#include <thread>

#include "Networking/Server.h"
#include "Objects/InvaderMatrix.h"
#include "Objects/SpawnPoint.h"

using namespace std;
using namespace sf;

// define objects
map<string, GameObject*> objects;
list<Collidable*> collidableObjects;
vector<SpawnPoint*> spawnPoints;

GameTime gameTime(1);

int main()
{
	// init event manager
	EventManager manager(gameTime);
	thread exeEvent(&EventManager::keepExecutingEvents, &manager);
	exeEvent.detach();

	// init spawn points
	SpawnPoint spawnPoint("SP1", &manager, Vector2f(400.f, 500.f));
	spawnPoints.emplace_back(&spawnPoint);

	// init invaders
	InvaderMatrix invaders(&manager, 2, 10,
		Vector2f(10.f, 10.f), Vector2f(300.f, 100.f), 700.f,
		Vector2f(50.f, 50.f), gameTime
	);

	// init server
	Server server(&objects, &manager);
	server.receiverHandler(&gameTime); // wait for client connection
	thread newThread(&Server::receiverHandler, &server, &gameTime);
	newThread.detach();

	// get character connected
	Character* character = dynamic_cast<Character*>(objects["A"]);
	character->setSpawnPoints(&spawnPoints);

	// timer
	double elapsed, thisTime, lastTime = gameTime.getTime();
	
	while (true)
	{
		// get time tic elapsed for this iteration
		thisTime = gameTime.getTime();
		elapsed = thisTime - lastTime;

		manager.getMtxQueue()->lock();

		// detect character collision, with invader, with invaders' bullet
		list<Collidable*> invadersList = invaders.getInvaderCList();
		dynamic_cast<Collidable*>(
			character->getGC(ComponentType::COLLIDABLE)
			)->work(invadersList, elapsed);

		list<Collidable*> inBulletsList = invaders.getBulletsCList();
		dynamic_cast<Collidable*>(
			character->getGC(ComponentType::COLLIDABLE)
			)->work(inBulletsList, elapsed);

		// detect invader collision, with character's bullet
		list<Collidable*> chBulletsList = character->getBulletsCList();
		for (Collidable* invader : invadersList)
		{
			invader->work(chBulletsList, elapsed);
		}

		// update GVT and execute events
		manager.executeEvents();

		// move invaders
		invaders.move(elapsed);

		manager.getMtxQueue()->unlock();

		// refresh time
		lastTime = thisTime;

		// publish messages
		server.publisherHandler(&invaders);

		// sleep for 16ms to avoid too frequent publish
		Sleep(16);
	}

	return 0;
}
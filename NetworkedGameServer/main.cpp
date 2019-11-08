#include <SFML/Graphics.hpp>
#include <thread>

#include "Networking/Server.h"

using namespace std;
using namespace sf;

// define objects
list<Collidable*> collidableObjects;
//list<SpawnPoint*> spawnPoints;
//list<DeathZone*> deathZones;

GameTime gameTime(1);

int main()
{
	// init event manager
	EventManager manager(gameTime, SERVER_NAME);

	// init server
	Server server(&manager);
	thread newThread(&Server::receiverHandler, &server, &gameTime);
	newThread.detach();

	// init platforms
	MovingPlatform platform(
		"SP1", ::Shape::RECTANGLE, ::Color::GREEN, Vector2f(400.f, 50.f), Vector2f(200.f, 400.f),
		Vector2f(0.f, 0.f), gameTime, Move::STATIC
	);
	collidableObjects.emplace_back(dynamic_cast<Collidable*>(platform.getGC(ComponentType::COLLIDABLE)));

	MovingPlatform movingPlatform(
		"MP1", ::Shape::RECTANGLE, ::Color::RED, Vector2f(400.f, 50.f), Vector2f(750.f, 320.f),
		Vector2f(100.f, 0.f), gameTime, Move::HORIZONTAL, 600.f, 200.f
	);
	collidableObjects.emplace_back(dynamic_cast<Collidable*>(movingPlatform.getGC(ComponentType::COLLIDABLE)));

	MovingPlatform verticalPlatform(
		"MP2", ::Shape::RECTANGLE, ::Color::RED, Vector2f(300.f, 50.f), Vector2f(1250.f, 220.f),
		Vector2f(0.f, 50.f), gameTime, Move::VERTICAL, 200.f, 50.f
	);
	collidableObjects.emplace_back(dynamic_cast<Collidable*>(verticalPlatform.getGC(ComponentType::COLLIDABLE)));

	//// init spawn points
	//SpawnPoint spawnPoint(Vector2f(400.f, 100.f));
	//spawnPoints.emplace_back(&spawnPoint);

	// timer
	double elapsed, thisTime, lastTime = gameTime.getTime();
	
	while (true)
	{
		// get time tic elapsed for this iteration
		thisTime = gameTime.getTime();
		elapsed = thisTime - lastTime;

		// move all moving platforms
		for (Collidable* moving : collidableObjects)
		{
			dynamic_cast<Movable*>(moving->getMovable())->work(elapsed);
		}

		// refresh time
		lastTime = thisTime;

		// publish messages
		server.publisherHandler(&collidableObjects);
	}

	return 0;
}
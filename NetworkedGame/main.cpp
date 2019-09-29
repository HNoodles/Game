#include <SFML/Graphics.hpp>
#include <thread>

#include "Networking/Server.h"

using namespace std;
using namespace sf;

// define objects
list<Movable*> movingObjects;
list<MovingPlatform*> platforms;
map<string, Character*> characters;

GameTime gameTime(1);

int main()
{
	cout << gameTime.getTime() << endl;

	// init server
	Server server(&platforms);
	thread newThread(&Server::receiverHandler, &server, &gameTime, &characters);
	newThread.detach();

	// init platforms
	MovingPlatform platform(Vector2f(200.f, 50.f), Vector2f(0.f, 0.f), 100.f, 100.f, gameTime);
	platform.setPosition(Vector2f(100.f, 400.f));
	movingObjects.emplace_back(&platform);
	platforms.emplace_back(&platform);

	MovingPlatform movingPlatform(Vector2f(200.f, 50.f), Vector2f(100.f, 0.f), 300.f, 200.f, gameTime);
	movingPlatform.setPosition(Vector2f(550.f, 300.f));
	movingObjects.emplace_back(&movingPlatform);
	platforms.emplace_back(&movingPlatform);

	// timer
	double elapsed, thisTime, lastTime = gameTime.getTime();
	
	// run the program as long as the window is open
	while (true)
	{
		// get time tic elapsed for this iteration
		thisTime = gameTime.getTime();
		elapsed = thisTime - lastTime;

		// detect character collision and move them
		for (auto pair : characters)
		{
			pair.second->detectCollision(platforms, elapsed);
			pair.second->update(elapsed);
		}

		// move all moving objects
		for (Movable* moving : movingObjects)
		{
			moving->update(elapsed);
		}

		lastTime = thisTime;

		// publish messages
		server.publisherHandler(&platforms);
	}

	return 0;
}
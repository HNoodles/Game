#include <SFML/Graphics.hpp>
#include <thread>

#include "Networking/Server.h"

using namespace std;
using namespace sf;

// define objects
list<MovingPlatform*> platforms;

GameTime gameTime(1);

int main()
{
	// init server
	Server server;
	thread newThread(&Server::receiverHandler, &server, &gameTime);
	newThread.detach();
	//server.receiverHandler(&gameTime);

	// init platforms
	MovingPlatform platform(Vector2f(200.f, 50.f), Vector2f(0.f, 0.f), 100.f, 100.f, gameTime);
	platform.setPosition(Vector2f(100.f, 400.f));
	platforms.emplace_back(&platform);

	MovingPlatform movingPlatform(Vector2f(200.f, 50.f), Vector2f(100.f, 0.f), 300.f, 200.f, gameTime);
	movingPlatform.setPosition(Vector2f(550.f, 300.f));
	platforms.emplace_back(&movingPlatform);

	// timer
	double elapsed, thisTime, lastTime = gameTime.getTime();
	
	while (true)
	{
		//server.receiverHandler(&gameTime);

		// get time tic elapsed for this iteration
		thisTime = gameTime.getTime();
		elapsed = thisTime - lastTime;

		// detect character collision and move them
		/*for (auto pair : characters)
		{
			pair.second->detectCollision(platforms, elapsed);
			pair.second->update(elapsed);
		}*/

		// move all moving platforms
		for (MovingPlatform* moving : platforms)
		{
			moving->update(elapsed);
		}

		lastTime = thisTime;

		// publish messages
		server.publisherHandler(&platforms);
	}

	return 0;
}
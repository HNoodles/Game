#include <SFML/Graphics.hpp>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <thread>
#include "Networking/Client.h"
#include "Objects/SpawnPoint.h"
#include "Objects/InvaderMatrix.h"

using namespace std;
using namespace sf;

void initWindow(Window& window);
void handleScalingOption(RenderWindow& window);
void handleGameInstruction(double& thisTime, EventManager* manager);
void handleWindowEvent(RenderWindow& window, Client* client);

// define objects
map<string, GameObject*> objects;
list<Renderable*> renders;
vector<SpawnPoint*> spawnPoints;

GameTime gameTime(1);

// scaling switch
bool isConstantScaling = false;
Vector2u lastWindowSize(800, 600);// default window size

mutex mtxObjMov;

int main()
{
	// declare and init window
	RenderWindow window;
	initWindow(window);

	// init event manager
	EventManager manager(&gameTime, &mtxObjMov);
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

	// init character
	Character character(
		SELF_NAME, &manager, 
		::Shape::DIAMOND, ::Color::BLUE, Vector2f(30.f, 60.f), 
		dynamic_cast<Renderable*>(spawnPoint.getGC(ComponentType::RENDERABLE))->getShape()->getPosition(),
		Vector2f(100.0f, 0.0f), gameTime, &spawnPoints
	);
	objects.insert({ character.getId(), &character });

	//// init client
	//Client client(&objects, &manager);

	//// send message to notify server this new client
	//client.connect();

	//// update platform and other character infos from server
	//thread newThread(&Client::subscribeHandler, &client, &gameTime);
	//newThread.detach();

	// timer
	double elapsed, thisTime, lastTime = gameTime.getTime();
	
	// run the program as long as the window is open
	while (window.isOpen())
	{
		// get time tic elapsed for this iteration
		thisTime = gameTime.getTime();
		elapsed = thisTime - lastTime;
		
		// only handle key events when window is focused
		if (window.hasFocus())
		{
			//// deal with events
			//handleWindowEvent(window, &client);

			// handle scaling option
			handleScalingOption(window);

			// handle game instruction
			handleGameInstruction(thisTime, &manager);
		}

		manager.getMtxQueue()->lock();

		// detect character collision, with invader, with invaders' bullet
		list<Collidable*> invadersList = invaders.getInvaderCList();
		dynamic_cast<Collidable*>(
			character.getGC(ComponentType::COLLIDABLE)
		)->work(invadersList, elapsed);

		list<Collidable*> inBulletsList = invaders.getBulletsCList();
		dynamic_cast<Collidable*>(
			character.getGC(ComponentType::COLLIDABLE)
		)->work(inBulletsList, elapsed);

		// detect invader collision, with character's bullet
		list<Collidable*> chBulletsList = character.getBulletsCList();
		for (Collidable* invader : invadersList)
		{
			invader->work(chBulletsList, elapsed);
		}

		// update GVT and execute events
		manager.executeEvents();

		if (window.hasFocus())
		{
			// move character, fire
			character.handleKeyInput();
		}

		// update GVT and execute events
		manager.executeEvents();

		// update character position
		character.move(elapsed);
		// move invaders
		invaders.move(elapsed);
		manager.getMtxQueue()->unlock();

		//// send message to notify server the update of this client
		//client.sendHandler();

		// clear the window with the chosen color
		window.clear(sf::Color::White);

		// draw the objects needed
		manager.getMtxQueue()->lock();
		list<Renderable*> iRenders = invaders.getRList();
		list<Renderable*> cRenders = character.getRList();
		// refresh
		renders.clear();
		renders.insert(renders.end(), iRenders.begin(), iRenders.end());
		renders.insert(renders.end(), cRenders.begin(), cRenders.end());

		for (Renderable* render : renders) 
		{
			lock_guard<mutex> guard(mtxObjMov);
			window.draw(*render->getShape());
		}
		manager.getMtxQueue()->unlock();

		// end of the current frame, show the window
		window.display();

		lastTime = thisTime;
	}

	return 0;
}

void initWindow(Window& window) {
	// anti-aliase
	ContextSettings settings;
	settings.antialiasingLevel = 8;

	// create the window
	window.create(
		VideoMode(lastWindowSize.x, lastWindowSize.y),
		"My Demo Window (Proportional)",
		Style::Default, // default = titleBar + resize + close
		settings
	);

	// turn on vertical sync
	window.setVerticalSyncEnabled(true);
}

void handleScalingOption(RenderWindow& window)
{
	if ((Keyboard::isKeyPressed(Keyboard::LControl) || Keyboard::isKeyPressed(Keyboard::RControl))
		&& Keyboard::isKeyPressed(Keyboard::C)) 
	{// set to constant scaling mode
		isConstantScaling = true;
		window.setTitle("My Demo Window (Constant)");
	}
	if ((Keyboard::isKeyPressed(Keyboard::LControl) || Keyboard::isKeyPressed(Keyboard::RControl))
		&& Keyboard::isKeyPressed(Keyboard::P))
	{// set to proportional scaling mode
		isConstantScaling = false;
		window.setTitle("My Demo Window (Proportional)");
	}
	if ((Keyboard::isKeyPressed(Keyboard::LControl) || Keyboard::isKeyPressed(Keyboard::RControl))
		&& Keyboard::isKeyPressed(Keyboard::R))
	{// return to normal
		isConstantScaling = false;
		window.setTitle("My Demo Window (Proportional)");
		Vector2u defaultSize(800, 600);
		window.setSize(defaultSize);
		window.setView(View(FloatRect(0, 0, (float)defaultSize.x, (float)defaultSize.y)));
	}
}

void handleGameInstruction(double & thisTime, EventManager* manager)
{
	if (Keyboard::isKeyPressed(Keyboard::P))
	{// switch paused status
		gameTime.setPaused(!gameTime.getPaused());
		Sleep(200);// sleep to avoid multi key events triggerd at a time
	}

	if ((Keyboard::isKeyPressed(Keyboard::LControl) || Keyboard::isKeyPressed(Keyboard::RControl))
		&& Keyboard::isKeyPressed(Keyboard::S))
	{// set to slow time speed
		gameTime.setStepSize(2);
		thisTime = gameTime.getTime();
	}
	if ((Keyboard::isKeyPressed(Keyboard::LControl) || Keyboard::isKeyPressed(Keyboard::RControl))
		&& Keyboard::isKeyPressed(Keyboard::N))
	{// set to normal time speed
		gameTime.setStepSize(1);
		thisTime = gameTime.getTime();
	}
	if ((Keyboard::isKeyPressed(Keyboard::LControl) || Keyboard::isKeyPressed(Keyboard::RControl))
		&& Keyboard::isKeyPressed(Keyboard::F))
	{// set to fast time speed
		gameTime.setStepSize(0.5);
		thisTime = gameTime.getTime();
	}
}

void handleWindowEvent(RenderWindow& window, Client* client) {
	// track all the window's events that were triggered since the last iteration
	sf::Event event;
	while (window.pollEvent(event))// returns true when there is event pending
	{// handle events in the loop
		switch (event.type)
		{
		case sf::Event::Closed:// close requested, then close the window
			client->disconnect();
			window.close();
			break;
		case sf::Event::Resized:// catch the resize events
			if (isConstantScaling)
			{// update the view to the new size of the window
				Vector2f oldViewSize = window.getView().getSize();
				FloatRect visibleArea(0, 0, oldViewSize.x * event.size.width / lastWindowSize.x, 
					oldViewSize.y * event.size.height / lastWindowSize.y);
				window.setView(View(visibleArea));
				lastWindowSize = window.getSize();
			}
			else
			{// else do the default proportional scaling
				lastWindowSize = window.getSize();
			}
			break;
		}
	}
}

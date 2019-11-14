#include <SFML/Graphics.hpp>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <thread>
#include "Networking/Client.h"
#include "Objects/MovingPlatform.h"
#include "Objects/SpawnPoint.h"
#include "Objects/DeathZone.h"
#include "Objects/SideBoundary.h"

using namespace std;
using namespace sf;

void initWindow(Window& window);
void handleScalingOption(RenderWindow& window);
void handleGameInstruction(double& thisTime, EventManager* manager, Replay* replay);
//void loadTextureFromFile(Texture& texture, string file_name);
//void loadTextures();
void handleWindowEvent(RenderWindow& window, Client* client);
void handleReplayInstruction(Replay* replay);

// define objects
//map<string, Texture> textures;
map<string, GameObject*> objects;
list<Collidable*> collidableObjects;
vector<SpawnPoint*> spawnPoints;
vector<SideBoundary*> sideBoundaries;

GameTime gameTime(1);

// scaling switch
bool isConstantScaling = false;
Vector2u lastWindowSize(800, 600);// default window size
Vector2u wholeSize(1600, 600); // whole view size

Vector2f renderOffset(0.f, 0.f);

mutex mtxObjMov;

int main()
{
	// declare and init window
	RenderWindow window;
	initWindow(window);
	
	//// load textures
	//loadTextures();

	// init replay system
	Replay replay(gameTime);

	// init event manager
	EventManager manager(gameTime, &mtxObjMov, &replay);
	thread exeEvent(&EventManager::keepExecutingEvents, &manager);
	exeEvent.detach();

	// set EM for replay
	replay.setEM(&manager);

	// init platforms
	MovingPlatform platform(
		"MP1", &manager, 
		::Shape::RECTANGLE, ::Color::GREEN, Vector2f(400.f, 50.f), Vector2f(200.f, 400.f),
		Vector2f(0.f, 0.f), gameTime, Move::HORIZONTAL
	);
	objects.insert({ platform.getId(), &platform });
	collidableObjects.emplace_back(dynamic_cast<Collidable*>(platform.getGC(ComponentType::COLLIDABLE)));

	MovingPlatform movingPlatform(
		"MP2", &manager, 
		::Shape::RECTANGLE, ::Color::RED, Vector2f(400.f, 50.f), Vector2f(750.f, 320.f),
		Vector2f(100.f, 0.f), gameTime, Move::HORIZONTAL, 600.f, 200.f
	);
	objects.insert({ movingPlatform.getId(), &movingPlatform });
	collidableObjects.emplace_back(dynamic_cast<Collidable*>(movingPlatform.getGC(ComponentType::COLLIDABLE)));

	MovingPlatform verticalPlatform(
		"MP3", &manager, 
		::Shape::RECTANGLE, ::Color::RED, Vector2f(300.f, 50.f), Vector2f(1250.f, 220.f),
		Vector2f(0.f, 50.f), gameTime, Move::VERTICAL, 200.f, 50.f
	);
	objects.insert({ verticalPlatform.getId(), &verticalPlatform });
	collidableObjects.emplace_back(dynamic_cast<Collidable*>(verticalPlatform.getGC(ComponentType::COLLIDABLE)));

	// init spawn points
	SpawnPoint spawnPoint("SP1", &manager, Vector2f(400.f, 100.f));
	spawnPoints.emplace_back(&spawnPoint);

	// init death zones
	DeathZone left("DZ1", &manager, 
		::Shape::RECTANGLE, Vector2f(1.f, (float)wholeSize.y), Vector2f(0.f, 0.f));
	DeathZone right("DZ2", &manager, 
		::Shape::RECTANGLE, Vector2f(1.f, (float)wholeSize.y), Vector2f((float)wholeSize.x, 0.f));
	DeathZone up("DZ3", &manager, 
		::Shape::RECTANGLE, Vector2f((float)wholeSize.x, 1.f), Vector2f(0.f, 0.f));
	DeathZone bottom("DZ4", &manager, 
		::Shape::RECTANGLE, Vector2f((float)wholeSize.x, 1.f), Vector2f(0.f, (float)wholeSize.y));
	collidableObjects.emplace_back(dynamic_cast<Collidable*>(left.getGC(ComponentType::COLLIDABLE)));
	collidableObjects.emplace_back(dynamic_cast<Collidable*>(right.getGC(ComponentType::COLLIDABLE)));
	collidableObjects.emplace_back(dynamic_cast<Collidable*>(up.getGC(ComponentType::COLLIDABLE)));
	collidableObjects.emplace_back(dynamic_cast<Collidable*>(bottom.getGC(ComponentType::COLLIDABLE)));

	// init side boundaries
	SideBoundary lsb("SB1", &manager, 
		::Direction::LEFT, lastWindowSize, 100.f, renderOffset, &sideBoundaries);
	SideBoundary rsb("SB2", &manager, 
		::Direction::RIGHT, lastWindowSize, 100.f, renderOffset, &sideBoundaries);
	sideBoundaries.emplace_back(&lsb);
	sideBoundaries.emplace_back(&rsb);
	collidableObjects.emplace_back(dynamic_cast<Collidable*>(lsb.getGC(ComponentType::COLLIDABLE)));
	collidableObjects.emplace_back(dynamic_cast<Collidable*>(rsb.getGC(ComponentType::COLLIDABLE)));

	// init character
	Character character(
		SELF_NAME, &manager, 
		::Shape::DIAMOND, ::Color::BLUE, Vector2f(60.f, 120.f), 
		dynamic_cast<Renderable*>(spawnPoint.getGC(ComponentType::RENDERABLE))->getShape()->getPosition(),
		Vector2f(250.0f, 0.0f), gameTime, &spawnPoints, &renderOffset, &sideBoundaries
	);
	objects.insert({ character.getId(), &character });

	// init client
	Client client(&objects, &manager, &replay);

	// set client for replay
	replay.setClient(&client);

	// send message to notify server this new client
	client.connect();

	// update platform and other character infos from server
	thread newThread(&Client::subscribeHandler, &client, &gameTime);
	newThread.detach();

	// get updated game time
	Movable* charMove = dynamic_cast<Movable*>(character.getGC(ComponentType::MOVABLE));

	// timer
	double elapsed, thisTime, lastTime = gameTime.getTime();
	
	// run the program as long as the window is open
	while (window.isOpen())
	{
		// get time tic elapsed for this iteration
		thisTime = gameTime.getTime();
		elapsed = thisTime - lastTime;

		// skip the iteration if is replaying
		if (replay.getIsPlaying())
		{
			if (window.hasFocus())
			{
				handleReplayInstruction(&replay);
			}

			// clear the window with the chosen color
			window.clear(sf::Color::White);

			// draw the objects needed
			manager.getMtxQueue()->lock();
			for (auto pair : objects)
			{
				sf::Shape* object = dynamic_cast<Renderable*>
					(pair.second->getGC(ComponentType::RENDERABLE))
					->getShape();

				lock_guard<mutex> guard(mtxObjMov);
				object->move(renderOffset);
				window.draw(*object);
				object->move(-renderOffset);
			}
			manager.getMtxQueue()->unlock();

			// end of the current frame, show the window
			window.display();

			lastTime = thisTime;
			continue;
		}

		// only handle key events when window is focused
		if (window.hasFocus())
		{
			// deal with events
			handleWindowEvent(window, &client);

			// handle scaling option
			handleScalingOption(window);

			// handle game instruction
			handleGameInstruction(thisTime, &manager, &replay);
		}

		// detect character collision
		manager.getMtxQueue()->lock();
		dynamic_cast<Collidable*>(
			character.getGC(ComponentType::COLLIDABLE)
		)->work(collidableObjects, elapsed);

		// update GVT and execute events
		manager.executeEvents();

		// set character out velocity
		character.setOutVelocity(elapsed);

		if (window.hasFocus())
		{
			// move character
			character.handleKeyInput();
		}

		// update GVT and execute events
		manager.executeEvents();

		// check if character has left the side boundary
		character.checkHitBoundary(&sideBoundaries);

		// update character position 
		charMove->work(elapsed);
		manager.getMtxQueue()->unlock();

		// send message to notify server the update of this client
		client.sendHandler();

		// clear the window with the chosen color
		window.clear(sf::Color::White);

		// draw the objects needed
		manager.getMtxQueue()->lock();
		for (auto pair : objects) 
		{
			sf::Shape* object = dynamic_cast<Renderable*>
				(pair.second->getGC(ComponentType::RENDERABLE))
				->getShape();

			lock_guard<mutex> guard(mtxObjMov);
			object->move(renderOffset);
			window.draw(*object);
			object->move(-renderOffset);
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

void handleGameInstruction(double & thisTime, EventManager* manager, Replay* replay)
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

	if (Keyboard::isKeyPressed(Keyboard::R))
	{// start recording
		manager->insertEvent(new EStartREC(gameTime.getTime(), replay));
	}
	if (Keyboard::isKeyPressed(Keyboard::E))
	{// end recording
		manager->insertEvent(new EEndREC(gameTime.getTime(), replay));
	}
}

//void loadTextureFromFile(Texture& texture, std::string file_name)
//{
//	if (!texture.loadFromFile(file_name))
//	{// fail to load
//		cout << "load texture failed" << endl;
//	}
//}
//
//void loadTextures()
//{
//	Texture grass, hero;
//
//	loadTextureFromFile(grass, "Images/space.png");
//	loadTextureFromFile(hero, "Images/hero.png");
//	
//	textures.insert(pair<string, Texture>("grass", grass));
//	textures.insert(pair<string, Texture>("hero", hero));
//}

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

void handleReplayInstruction(Replay* replay)
{
	if (Keyboard::isKeyPressed(Keyboard::N))
	{// normal speed
		replay->getReplayTime().setStepSize(1);
	}
	if (Keyboard::isKeyPressed(Keyboard::F))
	{// fast speed
		replay->getReplayTime().setStepSize(0.5);
	}
	if (Keyboard::isKeyPressed(Keyboard::S))
	{// slow speed
		replay->getReplayTime().setStepSize(2);
	}
}

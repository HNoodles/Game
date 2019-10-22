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
void handleGameInstruction(double & thisTime);
//void loadTextureFromFile(Texture& texture, string file_name);
//void loadTextures();
void handleWindowEvent(RenderWindow& window, Client* client);

// define objects
//map<string, Texture> textures;
list<sf::Shape*> objects;
list<Collidable*> collidableObjects;
map<string, Vector2f> characters;
list<MovingPlatform*> platforms;
vector<Renderable*> spawnPoints;
list<DeathZone*> deathZones;
vector<SideBoundary*> sideBoundaries;

GameTime gameTime(1);

// scaling switch
bool isConstantScaling = false;
Vector2u lastWindowSize(800, 600);// default window size
Vector2u wholeSize(1600, 600); // whole view size

Vector2f renderOffset(0.f, 0.f);

mutex l;

int main()
{
	// declare and init window
	RenderWindow window;
	initWindow(window);
	
	//// load textures
	//loadTextures();

	// init platforms
	MovingPlatform platform(
		::Shape::RECTANGLE, ::Color::GREEN, Vector2f(400.f, 50.f), Vector2f(200.f, 400.f),
		Vector2f(0.f, 0.f), gameTime, Move::HORIZONTAL
	);
	objects.emplace_back(dynamic_cast<Renderable*>(platform.getGC(ComponentType::RENDERABLE))->getShape());
	platforms.emplace_back(&platform);
	collidableObjects.emplace_back(dynamic_cast<Collidable*>(platform.getGC(ComponentType::COLLIDABLE)));

	MovingPlatform movingPlatform(
		::Shape::RECTANGLE, ::Color::RED, Vector2f(400.f, 50.f), Vector2f(750.f, 320.f),
		Vector2f(100.f, 0.f), gameTime, Move::HORIZONTAL, 600.f, 200.f
	);
	objects.emplace_back(dynamic_cast<Renderable*>(movingPlatform.getGC(ComponentType::RENDERABLE))->getShape());
	platforms.emplace_back(&movingPlatform);
	collidableObjects.emplace_back(dynamic_cast<Collidable*>(movingPlatform.getGC(ComponentType::COLLIDABLE)));

	MovingPlatform verticalPlatform(
		::Shape::RECTANGLE, ::Color::RED, Vector2f(300.f, 50.f), Vector2f(1250.f, 220.f),
		Vector2f(0.f, 50.f), gameTime, Move::VERTICAL, 200.f, 50.f
	);
	objects.emplace_back(dynamic_cast<Renderable*>(verticalPlatform.getGC(ComponentType::RENDERABLE))->getShape());
	platforms.emplace_back(&verticalPlatform);
	collidableObjects.emplace_back(dynamic_cast<Collidable*>(verticalPlatform.getGC(ComponentType::COLLIDABLE)));

	// init spawn points
	SpawnPoint spawnPoint(Vector2f(400.f, 100.f));
	spawnPoints.emplace_back(dynamic_cast<Renderable*>(spawnPoint.getGC(ComponentType::RENDERABLE)));

	// init death zones
	DeathZone left(::Shape::RECTANGLE, Vector2f(1.f, (float)wholeSize.y), Vector2f(0.f, 0.f));
	DeathZone right(::Shape::RECTANGLE, Vector2f(1.f, (float)wholeSize.y), Vector2f((float)wholeSize.x, 0.f));
	DeathZone up(::Shape::RECTANGLE, Vector2f((float)wholeSize.x, 1.f), Vector2f(0.f, 0.f));
	DeathZone bottom(::Shape::RECTANGLE, Vector2f((float)wholeSize.x, 1.f), Vector2f(0.f, (float)wholeSize.y));
	deathZones.emplace_back(&left);
	deathZones.emplace_back(&right);
	deathZones.emplace_back(&up);
	deathZones.emplace_back(&bottom);
	collidableObjects.emplace_back(dynamic_cast<Collidable*>(left.getGC(ComponentType::COLLIDABLE)));
	collidableObjects.emplace_back(dynamic_cast<Collidable*>(right.getGC(ComponentType::COLLIDABLE)));
	collidableObjects.emplace_back(dynamic_cast<Collidable*>(up.getGC(ComponentType::COLLIDABLE)));
	collidableObjects.emplace_back(dynamic_cast<Collidable*>(bottom.getGC(ComponentType::COLLIDABLE)));

	// init side boundaries
	SideBoundary lsb(::Direction::LEFT, lastWindowSize, 100.f);
	SideBoundary rsb(::Direction::RIGHT, lastWindowSize, 100.f);
	sideBoundaries.emplace_back(&lsb);
	sideBoundaries.emplace_back(&rsb);
	collidableObjects.emplace_back(dynamic_cast<Collidable*>(lsb.getGC(ComponentType::COLLIDABLE)));
	collidableObjects.emplace_back(dynamic_cast<Collidable*>(rsb.getGC(ComponentType::COLLIDABLE)));

	// init character
	Character character(
		::Shape::DIAMOND, ::Color::BLUE, Vector2f(60.f, 120.f), 
		dynamic_cast<Renderable*>(spawnPoint.getGC(ComponentType::RENDERABLE))->getShape()->getPosition(),
		Vector2f(250.0f, 0.0f), gameTime, &spawnPoints
	);
	objects.emplace_back(dynamic_cast<Renderable*>(character.getGC(ComponentType::RENDERABLE))->getShape());

	// init client
	Client client(&character, &characters, &l);

	// send message to notify server this new client
	client.sendHandler();

	// update platform and other character infos from server
	thread newThread(&Client::subscribeHandler, &client, &platforms);
	newThread.detach();

	// get updated game time
	Movable* charMove = dynamic_cast<Movable*>(character.getGC(ComponentType::MOVABLE));
	gameTime = *dynamic_cast<GameTime*>(
		&(charMove->getTimeline())
	);

	// timer
	double elapsed, thisTime, lastTime = gameTime.getTime();
	
	// run the program as long as the window is open
	while (window.isOpen())
	{
		// get updated game time
		gameTime = *dynamic_cast<GameTime*>(
			&(charMove->getTimeline())
		);

		// get time tic elapsed for this iteration
		thisTime = gameTime.getTime();
		elapsed = thisTime - lastTime;

		// only handle key events when window is focused
		if (window.hasFocus())
		{
			// deal with events
			handleWindowEvent(window, &client);

			// handle scaling option
			handleScalingOption(window);

			// handle game instruction
			handleGameInstruction(thisTime);
		}

		// detect character collision
		dynamic_cast<Collidable*>(
			character.getGC(ComponentType::COLLIDABLE)
		)->work(collidableObjects, elapsed, renderOffset, &sideBoundaries);

		if (window.hasFocus())
		{
			// move character
			character.handleKeyInput();
		}

		// update character position 
		charMove->work(elapsed);

		// send message to notify server the update of this client
		client.sendHandler();

		// clear the window with the chosen color
		window.clear(sf::Color::White);

		// draw the objects needed
		for (auto pair : characters) 
		{
			Character toDraw(
				::Shape::DIAMOND, ::Color::BLUE, Vector2f(60.f, 120.f), pair.second + renderOffset,
				Vector2f(250.0f, 0.0f), gameTime
			);
			window.draw(
				*dynamic_cast<Renderable*>(toDraw.getGC(ComponentType::RENDERABLE))->getShape()
			);
		}
		for (sf::Shape* object : objects) 
		{
			lock_guard<mutex> guard(l);
			object->move(renderOffset);
			window.draw(*object);
			object->move(-renderOffset);
		}

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

void handleGameInstruction(double & thisTime)
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
	Event event;
	while (window.pollEvent(event))// returns true when there is event pending
	{// handle events in the loop
		switch (event.type)
		{
		case Event::Closed:// close requested, then close the window
			client->disconnect();
			window.close();
			break;
		case Event::Resized:// catch the resize events
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
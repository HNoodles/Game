#include <SFML/Graphics.hpp>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "Networking/Client.h"
#include <thread>

using namespace std;
using namespace sf;

void initWindow(Window& window);
void handleScalingOption(RenderWindow& window);
void handleGameInstruction(double & thisTime);
void loadTextureFromFile(Texture& texture, string file_name);
void loadTextures();
void handleWindowEvent(RenderWindow& window);

// define objects
map<string, Texture> textures;
list<Drawable*> objects;
list<MovingPlatform*> platforms;
map<string, Vector2f> characters;

GameTime gameTime(1);

// scaling switch
bool isConstantScaling = false;
Vector2u lastWindowSize(800, 600);// default window size

// write switch
bool ableWriting = true;

int main()
{
	// declare and init window
	RenderWindow window;
	initWindow(window);
	
	// load textures
	loadTextures();

	//// init platforms
	//MovingPlatform platform(Vector2f(200.f, 50.f), Vector2f(0.f, 0.f), 100.f, 100.f, gameTime);
	//platform.setTexture(&textures["grass"], true);
	//objects.emplace_back(&platform);
	//platforms.emplace_back(&platform);

	//MovingPlatform movingPlatform(Vector2f(200.f, 50.f), Vector2f(100.f, 0.f), 300.f, 200.f, gameTime);
	//movingPlatform.setTexture(&textures["grass"], true);
	//objects.emplace_back(&movingPlatform);
	//platforms.emplace_back(&movingPlatform);

	// init character
	Character character(Vector2f(250.0f, 0.0f), gameTime);
	character.setTexture(&textures["hero"], true);
	character.setPosition(Vector2f(200.f, 100.f));
	objects.emplace_back(&character);

	// init client
	Client client(&character, &characters);

	// send message to notify server this new client
	client.sendHandler();

	// update platform and other character infos from server
	thread newThread(&Client::subscribeHandler, &client, &platforms);
	newThread.detach();

	// get updated game time
	gameTime = *dynamic_cast<GameTime*>(&character.getTimeline());

	// timer
	double elapsed, thisTime, lastTime = gameTime.getTime();
	
	// run the program as long as the window is open
	while (window.isOpen())
	{
		// get updated game time
		gameTime = *dynamic_cast<GameTime*>(&character.getTimeline());

		// get time tic elapsed for this iteration
		thisTime = gameTime.getTime();
		elapsed = thisTime - lastTime;

		// only handle key events when window is focused
		if (window.hasFocus())
		{
			// deal with events
			handleWindowEvent(window);

			// handle scaling option
			handleScalingOption(window);

			// handle game instruction
			handleGameInstruction(thisTime);
		}

		// detect character collision
		character.detectCollision(platforms, elapsed);

		if (window.hasFocus())
		{
			// move character
			character.handleKeyInput();
		}

		// update character position 
		character.update(elapsed);

		// send message to notify server the update of this client
		client.sendHandler();

		// clear the window with the chosen color
		window.clear(Color::White);

		// draw the objects needed
		for (auto pair : characters) 
		{
			Character toDraw(Vector2f(250.f, 0.f), gameTime);
			toDraw.setPosition(pair.second);
			toDraw.setTexture(&textures["hero"], true);
			window.draw(toDraw);
		}
		for (const Drawable* object : objects) 
		{
			window.draw(*object);
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
		VideoMode(800, 600),
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

void loadTextureFromFile(Texture& texture, std::string file_name)
{
	if (!texture.loadFromFile(file_name))
	{// fail to load
		cout << "load texture failed" << endl;
	}
}

void loadTextures()
{
	Texture grass, hero;

	loadTextureFromFile(grass, "Images/space.png");
	loadTextureFromFile(hero, "Images/hero.png");
	
	textures.insert(pair<string, Texture>("grass", grass));
	textures.insert(pair<string, Texture>("hero", hero));
}

void handleWindowEvent(RenderWindow& window) {
	// track all the window's events that were triggered since the last iteration
	Event event;
	while (window.pollEvent(event))// returns true when there is event pending
	{// handle events in the loop
		switch (event.type)
		{
		case Event::Closed:// close requested, then close the window
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
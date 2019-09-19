#include <SFML/Graphics.hpp>
#include <list>
#include <map>
#include <iostream>

#include "Objects/MovingPlatform.h"
#include "Objects/Character.h"

using namespace std;
using namespace sf;

void initWindow(Window& window);
void handleScalingOption(RenderWindow& window);
void loadTextureFromFile(Texture& texture, string file_name);
//void initPlatforms();
void loadTextures();
void handleWindowEvent(RenderWindow& window);

// define objects
map<string, Texture> textures;
list<Drawable*> objects;
list<Movable*> movingObjects;
list<MovingPlatform*> platforms;

// scaling switch
bool isConstantScaling = false;
Vector2u lastWindowSize(800, 600);// default window size

int main()
{
	// declare and init window
	RenderWindow window;
	initWindow(window);
	
	// load textures
	loadTextures();

	// init platforms
	MovingPlatform platform(Vector2f(200.f, 50.f), Vector2f(0.f, 0.f), 100.f, 100.f);
	platform.setTexture(&textures["grass"], true);
	platform.setPosition(Vector2f(100.f, 400.f));
	objects.emplace_back(&platform);
	movingObjects.emplace_back(&platform);
	platforms.emplace_back(&platform);

	MovingPlatform movingPlatform(Vector2f(200.f, 50.f), Vector2f(1.f, 0.f), 300.f, 200.f);
	movingPlatform.setTexture(&textures["grass"], true);
	movingPlatform.setPosition(Vector2f(550.f, 300.f));
	objects.emplace_back(&movingPlatform);
	movingObjects.emplace_back(&movingPlatform);
	platforms.emplace_back(&movingPlatform);

	// init character
	Character character;
	character.setTexture(&textures["hero"], true);
	character.setPosition(Vector2f(550.f, 100.f));
	objects.emplace_back(&character);
	movingObjects.emplace_back(&character);
	
	// run the program as long as the window is open
	while (window.isOpen())
	{
		// deal with events
		handleWindowEvent(window);

		// handle scaling option
		handleScalingOption(window);

		// clear the window with the chosen color
		window.clear(Color::White);

		// detect character collision
		character.detectCollision(platforms);

		// move all moving objects
		for (Movable* moving : movingObjects)
		{
			moving->around(window);
		}

		// draw the objects needed
		for (const Drawable* object : objects) 
		{
			window.draw(*object);
		}

		// end of the current frame, show the window
		window.display();
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
		window.setView(View(FloatRect(0, 0, defaultSize.x, defaultSize.y)));
	}
}

void loadTextureFromFile(Texture& texture, std::string file_name)
{
	if (!texture.loadFromFile(file_name))
	{// fail to load
		cout << "load texture failed" << endl;
	}
}

//void initPlatforms() 
//{
//	MovingPlatform platform(Vector2f(200.f, 50.f), Vector2f(0.f, 0.f), 100.f, 100.f);
//	platform.setTexture(&textures["grass"], true);
//	platform.setPosition(Vector2f(100.f, 400.f));
//	objects.emplace_back(&platform);
//	movingObjects.emplace_back(&platform);
//	platforms.emplace_back(&platform);
//
//	MovingPlatform movingPlatform(Vector2f(200.f, 50.f), Vector2f(1.f, 0.f), 300.f, 200.f);
//	movingPlatform.setTexture(&textures["grass"], true);
//	movingPlatform.setPosition(Vector2f(550.f, 300.f));
//	objects.emplace_back(&movingPlatform);
//	movingObjects.emplace_back(&movingPlatform);
//	platforms.emplace_back(&movingPlatform);
//}

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
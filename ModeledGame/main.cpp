#include <iostream>
#include <windows.h>

#include "Objects/MovingPlatform.h"
#include "Objects/Character.h"
#include "Times/GameTime.h"

void initWindow(Window& window);
void handleScalingOption(RenderWindow& window);
void handleGameInstruction(double & thisTime);
//void loadTextureFromFile(Texture& texture, string file_name);
//void loadTextures();
void handleWindowEvent(RenderWindow& window);

// define objects
//map<string, Texture> textures;
list<Drawable*> objects;
list<Movable*> movingObjects;
list<Collidable*> collidableObjects;

GameTime gameTime(1);

// scaling switch
bool isConstantScaling = false;
Vector2u lastWindowSize(800, 600);// default window size

int main()
{
	cout << gameTime.getTime() << endl;
	// declare and init window
	RenderWindow window;
	initWindow(window);
	
	// load textures
	//loadTextures();

	// init platforms
	MovingPlatform platform(
		::Shape::RECTANGLE, ::Color::GREEN, Vector2f(200.f, 50.f), Vector2f(100.f, 400.f), 
		Vector2f(0.f, 0.f), gameTime, Move::HORIZONTAL
	);
	objects.emplace_back(dynamic_cast<Renderable*>(platform.getGC(ComponentType::RENDERABLE))->getShape());
	collidableObjects.emplace_back(dynamic_cast<Collidable*>(platform.getGC(ComponentType::COLLIDABLE)));

	MovingPlatform movingPlatform(
		::Shape::RECTANGLE, ::Color::RED, Vector2f(200.f, 50.f), Vector2f(450.f, 320.f), 
		Vector2f(100.f, 0.f), gameTime, Move::HORIZONTAL, 300.f, 200.f
	);
	objects.emplace_back(dynamic_cast<Renderable*>(movingPlatform.getGC(ComponentType::RENDERABLE))->getShape());
	movingObjects.emplace_back(dynamic_cast<Movable*>(movingPlatform.getGC(ComponentType::MOVABLE)));
	collidableObjects.emplace_back(dynamic_cast<Collidable*>(movingPlatform.getGC(ComponentType::COLLIDABLE)));

	MovingPlatform verticalPlatform(
		::Shape::RECTANGLE, ::Color::RED, Vector2f(200.f, 50.f), Vector2f(550.f, 220.f), 
		Vector2f(0.f, 100.f), gameTime, Move::VERTICAL, 200.f, 50.f
	);
	objects.emplace_back(dynamic_cast<Renderable*>(verticalPlatform.getGC(ComponentType::RENDERABLE))->getShape());
	movingObjects.emplace_back(dynamic_cast<Movable*>(verticalPlatform.getGC(ComponentType::MOVABLE)));
	collidableObjects.emplace_back(dynamic_cast<Collidable*>(verticalPlatform.getGC(ComponentType::COLLIDABLE)));

	// init character
	Character character(
		::Shape::DIAMOND, ::Color::BLUE, Vector2f(60.f, 120.f), Vector2f(200.f, 100.f), 
		Vector2f(250.0f, 0.0f), gameTime
	);
	objects.emplace_back(dynamic_cast<Renderable*>(character.getGC(ComponentType::RENDERABLE))->getShape());
	movingObjects.emplace_back(dynamic_cast<Movable*>(character.getGC(ComponentType::MOVABLE)));

	// timer
	double elapsed, thisTime, lastTime = gameTime.getTime();
	
	// run the program as long as the window is open
	while (window.isOpen())
	{
		// get time tic elapsed for this iteration
		thisTime = gameTime.getTime();
		elapsed = thisTime - lastTime;
		
		// deal with events
		handleWindowEvent(window);

		// handle scaling option
		handleScalingOption(window);

		// handle game instruction
		handleGameInstruction(thisTime);

		// detect character collision
		Collidable* co = dynamic_cast<Collidable*>(character.getGC(ComponentType::COLLIDABLE));
		co->work(collidableObjects, elapsed);

		// move all moving objects
		for (Movable* moving : movingObjects)
		{
			moving->work(elapsed);
		}

		// clear the window with the chosen color
		window.clear(sf::Color::White);

		// draw the objects needed
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
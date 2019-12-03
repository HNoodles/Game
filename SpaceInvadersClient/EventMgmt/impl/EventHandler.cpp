#include "../EventManager.h"
#include "../../Objects/Character.h"
#include "../../Objects/SpawnPoint.h"
#include "../../Objects/Invader.h"
#include "../../Objects/InvaderMatrix.h"

void EventHandler::onCharDeath(ECharDeath e)
{
	Character* character = e.getCharacter();
	vector<SpawnPoint*>* spawnPoints = character->getSpawnPoints();

	// randomly select a spawn point to respawn
	int index = rand() % spawnPoints->size();
	
	// generate a character respawn event in manager
	manager->insertEvent(
		new ECharSpawn(gameTime->getTime(), character, (*spawnPoints)[index])
	);
}

void EventHandler::onInvaDeath(EInvaDeath e)
{
	Invader* invader = e.getInvader();
	invader->getMatrix()->kill(invader);
}

void EventHandler::onCharSpawn(ECharSpawn e)
{
	Character* character = e.getCharacter();
	SpawnPoint* spawnPoint = e.getSpawnPoint();

	// get spawn point's position
	Vector2f point = dynamic_cast<Renderable*>(spawnPoint->getGC(ComponentType::RENDERABLE))
		->getShape()->getPosition();

	// set character's position to it
	manager->insertEvent(new EObjMovement(gameTime->getTime(), character, point.x, point.y));

	// refresh out velocity
	character->getOutVelocity() = Vector2f(0.f, 0.f);
}

void EventHandler::onObjMovement(EObjMovement e)
{
	//// use scripting
	//s_manager.onObjMovement(e);

	GameObject* object = e.getObject();
	Vector2f movement = Vector2f((float)e.getXVal(), (float)e.getYVal());

	// move the object toward movement
	dynamic_cast<Renderable*>(object->getGC(ComponentType::RENDERABLE))
		->getShape()->setPosition(movement);
}

void EventHandler::onUserInput(EUserInput e)
{
	Character* character = e.getCharacter();
	Keyboard::Key keyPressed = e.getInputKey();

	Vector2f velocity = dynamic_cast<Movable*>(character->getGC(ComponentType::MOVABLE))->getVelocity();
	Vector2f& outVelocity = character->getOutVelocity();

	// calculate total velocity
	if (keyPressed == Keyboard::Left || keyPressed == Keyboard::A)
	{// left
		outVelocity -= velocity;
	}
	if (keyPressed == Keyboard::Right || keyPressed == Keyboard::D)
	{// right
		outVelocity += velocity;
	}
	if (Keyboard::isKeyPressed(Keyboard::Space))
	{// fire
		character->fire();
	}
}

EventHandler::EventHandler(Timeline* gameTime, EventManager* manager)
	: gameTime(gameTime), manager(manager)
{
}

void EventHandler::onEvent(const ::Event* e)
{
	switch (e->getType())
	{
	case Event_t::CHAR_DEATH:
		onCharDeath(*(ECharDeath*)e);
		break;
	case Event_t::INVADER_DEATH:
		onInvaDeath(*(EInvaDeath*)e);
		break;
	case Event_t::CHAR_SPAWN:
		onCharSpawn(*(ECharSpawn*)e);
		break;
	case Event_t::USER_INPUT:
		onUserInput(*(EUserInput*)e);
		break;
	case Event_t::OBJ_MOVEMENT:
		onObjMovement(*(EObjMovement*)e);
		break;
	default:
		break;
	}
}

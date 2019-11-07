#include "../EventHandler.h"
#include "../../Objects/SideBoundary.h"

void EventHandler::onCharCollision(ECharCollision e)
{
	Character* character = e.getCharacter();
	GameObject* object = e.getCollidableObj();

	Collidable* collidable = dynamic_cast<Collidable*>(object->getGC(ComponentType::COLLIDABLE));

	switch (collidable->getType())
	{
	case Collision::PLATFORM:
		platformWork(character, (MovingPlatform*)object);
		break;
	case Collision::DEATHZONE:
		// TODO: generate new ECharDeath in manager
		break;
	case Collision::SIDEBOUNDARY:
		sideBoundaryWork((SideBoundary*)object);
		break;
	default:
		break;
	}
}

void EventHandler::platformWork(Character* character, MovingPlatform* platform)
{
	// get boundary pointers of character
	vector<Collidable*>* boundary_ptrs = character->getBoundaryPtrs();
	
	// get four boundaries of character
	vector<RectangleShape> boundary_lines = dynamic_cast<Renderable*>(character->getGC(ComponentType::RENDERABLE))
		->getBoundaryLines();

	// get global bounds of platform
	FloatRect bound = dynamic_cast<Renderable*>(platform->getGC(ComponentType::RENDERABLE))
		->getShape()->getGlobalBounds();

	// get collidable of plaform
	Collidable* collidable = dynamic_cast<Collidable*>(platform->getGC(ComponentType::COLLIDABLE));

	// check collision using loop
	for (size_t i = 0; i < boundary_lines.size(); i++) {
		if (boundary_lines[i].getGlobalBounds().intersects(bound))
		{// collision!
			boundary_ptrs->at(i) = collidable;
		}
	}
}

void EventHandler::sideBoundaryWork(SideBoundary* boundary)
{
	// add offset this time to overall offset
	Vector2f offset = boundary->getOffset();
	boundary->getRenderOffset() += offset;

	// update the position of all the sideBoundaries
	for (SideBoundary* boundary : *boundary->getSideBoundaries())
	{
		boundary->updatePos(offset);
	}
}

void EventHandler::onCharDeath(ECharDeath e)
{
	Character* character = e.getCharacter();
	vector<SpawnPoint*>* spawnPoints = character->getSpawnPoints();

	// randomly select a spawn point to respawn
	int index = rand() % spawnPoints->size();
	
	// TODO: generate a character respawn event in manager
}

void EventHandler::onCharSpawn(ECharSpawn e)
{
	Character* character = e.getCharacter();
	SpawnPoint* spawnPoint = e.getSpawnPoint();

	// get spawn point's position
	Vector2f point = dynamic_cast<Renderable*>(spawnPoint->getGC(ComponentType::RENDERABLE))
		->getShape()->getPosition();

	// set character's position to it
	dynamic_cast<Renderable*>(character->getGC(ComponentType::RENDERABLE))
		->getShape()->setPosition(point);
}

void EventHandler::onObjMovement(EObjMovement e)
{
	GameObject* object = e.getObject();
	Vector2f movement = Vector2f(e.getXVal(), e.getYVal());

	// move the object toward movement
	dynamic_cast<Renderable*>(object->getGC(ComponentType::RENDERABLE))
		->getShape()->move(movement);
}

void EventHandler::onUserInput(EUserInput e)
{
	Character* character = e.getCharacter();
	Keyboard::Key keyPressed = e.getInputKey();

	Vector2f velocity = dynamic_cast<Movable*>(character->getGC(ComponentType::MOVABLE))->getVelocity();
	Vector2f& outVelocity = character->getOutVelocity();
	vector<Collidable*> boundary_ptrs = *character->getBoundaryPtrs();

	// calculate total velocity
	if (Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A))
	{// left
		outVelocity -= velocity;
	}
	if (Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D))
	{// right
		outVelocity += velocity;
	}
	if ((Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W))
		&& boundary_ptrs[3] != nullptr) // character should be on a platform to jump
	{// jump
		outVelocity.y = -300.f;
	}
}

EventHandler::EventHandler(GameTime& gameTime) : gameTime(gameTime)
{
}

void EventHandler::onEvent(const ::Event* e)
{
	switch (e->getType())
	{
	case Event_t::CHAR_COLLISION:
		onCharCollision(*(ECharCollision*)e);
		break;
	case Event_t::CHAR_DEATH:
		onCharDeath(*(ECharDeath*)e);
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
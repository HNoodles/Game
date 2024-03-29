#pragma once
#include "Event.h"
#include "../Components/Collidable.h"

class ECharSpawn : public ::Event
{
public:
	ECharSpawn(double executeTime, Character* character, SpawnPoint* spawnPoint);

	Character* getCharacter()
	{
		return *(Character**)getArg(Content_t::CHARACTER_PTR).getVal(Variant_t::OBJ_PTR);
	}

	SpawnPoint* getSpawnPoint()
	{
		return *(SpawnPoint**)getArg(Content_t::OBJ_PTR).getVal(Variant_t::OBJ_PTR);
	}
};


#pragma once
#include "MyEvent.h"
#include "../Objects/Character.h"

class ECharCollision : public ::MyEvent
{
public:
	ECharCollision(double executeTime, Character* character, GameObject* collidableObj);

	Character* getCharacter()
	{
		return (Character*)getArg(Content_t::CHARACTER_PTR).getVal(Variant_t::OBJ_PTR);
	}

	GameObject* getCollidableObj()
	{
		return (GameObject*)getArg(Content_t::OBJ_PTR).getVal(Variant_t::OBJ_PTR);
	}
};


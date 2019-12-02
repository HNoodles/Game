#pragma once
#include "Event.h"
#include "../Components/Collidable.h"

class ECharDeath : public ::Event
{
public:
	ECharDeath(double executeTime, Character* character);

	Character* getCharacter()
	{
		return *(Character**)getArg(Content_t::CHARACTER_PTR).getVal(Variant_t::OBJ_PTR);
	}
};


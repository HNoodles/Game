#pragma once
#include "Event.h"
#include "../Components/Collidable.h"

class EUserInput : public ::Event
{
public:
	EUserInput(double executeTime, Character* character, Keyboard::Key input);

	Character* getCharacter()
	{
		return *(Character**)getArg(Content_t::CHARACTER_PTR).getVal(Variant_t::OBJ_PTR);
	}

	Keyboard::Key getInputKey()
	{
		return *(Keyboard::Key*)getArg(Content_t::USER_INPUT).getVal(Variant_t::KEY);
	}
};


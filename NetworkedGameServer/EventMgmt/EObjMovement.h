#pragma once
#include "Event.h"
#include "../Objects/Character.h"

class EObjMovement : public ::Event
{
public:
	EObjMovement(double executeTime, Character* character, double x_val, double y_val);

	Character* getCharacter()
	{
		return (Character*)getArg(Content_t::CHARACTER_PTR).getVal(Variant_t::OBJ_PTR);
	}

	double getXVal()
	{
		return *(double*)getArg(Content_t::X_VAL).getVal(Variant_t::DOUBLE);
	}

	double getYVal()
	{
		return *(double*)getArg(Content_t::Y_VAL).getVal(Variant_t::DOUBLE);
	}
};


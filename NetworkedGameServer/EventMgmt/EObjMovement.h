#pragma once
#include "Event.h"

class EObjMovement : public ::Event
{
public:
	EObjMovement(double executeTime, GameObject* character, double x_val, double y_val);

	GameObject* getObject()
	{
		return (GameObject*)getArg(Content_t::OBJ_PTR).getVal(Variant_t::OBJ_PTR);
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


#pragma once
#include "Event.h"

class EObjMovement : public ::Event
{
public:
	EObjMovement(double executeTime, GameObject* object, 
		double x_val, double y_val, bool positive = false, double offset_x = 0, double offset_y = 0);

	EObjMovement() : Event(Event_t::OBJ_MOVEMENT, 0) {};

	GameObject* getObject()
	{
		return *(GameObject**)getArg(Content_t::OBJ_PTR).getVal(Variant_t::OBJ_PTR);
	}

	double getXVal()
	{
		return *(double*)getArg(Content_t::X_VAL).getVal(Variant_t::DOUBLE);
	}

	double getYVal()
	{
		return *(double*)getArg(Content_t::Y_VAL).getVal(Variant_t::DOUBLE);
	}

	bool getPositive()
	{
		return *(bool*)getArg(Content_t::POSITIVE).getVal(Variant_t::BOOL);
	}

	double getOffsetX()
	{
		return *(double*)getArg(Content_t::OFFSET_X).getVal(Variant_t::DOUBLE);
	}

	double getOffsetY()
	{
		return *(double*)getArg(Content_t::OFFSET_Y).getVal(Variant_t::DOUBLE);
	}

	string toString();
};


#pragma once
#include "Event.h"
#include "../Components/Collidable.h"

class EInvaDeath : public ::Event
{
public:
	EInvaDeath(double executeTime, Invader* invader);

	Invader* getInvader()
	{
		return *(Invader**)getArg(Content_t::INVADER_PTR).getVal(Variant_t::OBJ_PTR);
	}
};


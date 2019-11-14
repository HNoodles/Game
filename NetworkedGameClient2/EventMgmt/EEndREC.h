#pragma once
#include "Event.h"
#include "../Replay/Replay.h"

class EEndREC : public ::Event
{
public:
	EEndREC(double executeTime, Replay* replay);

	Replay* getReplay()
	{
		return *(Replay**)getArg(Content_t::REPLAY_PTR).getVal(Variant_t::REPLAY_PTR);
	}
};


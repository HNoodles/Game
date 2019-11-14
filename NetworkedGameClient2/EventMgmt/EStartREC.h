#pragma once
#include "Event.h"
#include "../Replay/Replay.h"

class EStartREC : public ::Event
{
public:
	EStartREC(double executeTime, Replay* replay);

	Replay* getReplay()
	{
		return *(Replay**)getArg(Content_t::REPLAY_PTR).getVal(Variant_t::REPLAY_PTR);
	}
};


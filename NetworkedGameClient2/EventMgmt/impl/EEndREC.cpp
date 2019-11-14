#include "../EEndREC.h"

EEndREC::EEndREC(double executeTime, Replay* replay) : Event(Event_t::END_REC, executeTime)
{
	addArg(Content_t::REPLAY_PTR, Variant_t::REPLAY_PTR, &replay);
}

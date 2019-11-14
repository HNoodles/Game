#include "../EStartREC.h"

EStartREC::EStartREC(double executeTime, Replay* replay) : Event(Event_t::START_REC, executeTime)
{
	addArg(Content_t::REPLAY_PTR, Variant_t::REPLAY_PTR, &replay);
}

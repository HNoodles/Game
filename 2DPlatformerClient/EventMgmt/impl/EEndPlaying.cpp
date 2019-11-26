#include "../EEndPlaying.h"

EEndPlaying::EEndPlaying(double executeTime, Replay* replay) : Event(Event_t::END_PLAY, executeTime)
{
	addArg(Content_t::REPLAY_PTR, Variant_t::REPLAY_PTR, &replay);
}

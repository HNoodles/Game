#include "../EInvaDeath.h"

EInvaDeath::EInvaDeath(double executeTime, Invader* invader)
	: Event(Event_t::INVADER_DEATH, executeTime)
{
	addArg(Content_t::INVADER_PTR, Variant_t::OBJ_PTR, &invader);
}

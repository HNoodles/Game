#include "../Event.h"

void ::Event::addArg(Content_t content_t, Variant_t variant_t, void* arg)
{
	args.insert({ content_t, Variant(variant_t, arg) });
}

::Event::Event(Event_t type, double executeTime) : type(type), executeTime(executeTime)
{
}

#include "../Event.h"

::Event::Event(Event_t type, double executeTime) : type(type), executeTime(executeTime)
{
}

bool ::Event::operator>(const Event& rv) const
{
	return this->executeTime > rv.executeTime;
}

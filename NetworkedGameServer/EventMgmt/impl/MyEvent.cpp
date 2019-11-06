#include "../MyEvent.h"

::MyEvent::MyEvent(Event_t type, double executeTime) : type(type), executeTime(executeTime)
{
}

bool ::MyEvent::operator>(const MyEvent& rv) const
{
	return this->executeTime > rv.executeTime;
}

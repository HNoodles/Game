#include "../ECharDeath.h"

ECharDeath::ECharDeath(double executeTime, Character* character)
	: Event(Event_t::CHAR_DEATH, executeTime)
{
	addArg(Content_t::CHARACTER_PTR, Variant_t::OBJ_PTR, &character);
}

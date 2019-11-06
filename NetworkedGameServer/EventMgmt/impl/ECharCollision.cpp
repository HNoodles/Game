#include "../ECharCollision.h"

ECharCollision::ECharCollision(double executeTime, Character* character, GameObject* collidableObj)
	: MyEvent(Event_t::CHAR_COLLISION, executeTime)
{
	addArg(Content_t::CHARACTER_PTR, Variant_t::OBJ_PTR, character);
	addArg(Content_t::OBJ_PTR, Variant_t::OBJ_PTR, collidableObj);
}

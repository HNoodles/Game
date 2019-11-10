#include "../ECharSpawn.h"

ECharSpawn::ECharSpawn(double executeTime, Character* character, SpawnPoint* spawnPoint)
	: Event(Event_t::CHAR_SPAWN, executeTime)
{
	addArg(Content_t::CHARACTER_PTR, Variant_t::OBJ_PTR, &character);
	addArg(Content_t::OBJ_PTR, Variant_t::OBJ_PTR, &spawnPoint);
}

#include "../../Objects/GameObject.h"
#include "../../Replay/Replay.h"

Variant::Variant(Variant_t type, void* value) : type(type), var()
{
	switch (type)
	{
	case Variant_t::DOUBLE:
		var.double_val = *(double*)value;
		break;
	case Variant_t::OBJ_PTR:
		var.obj_ptr = *(GameObject**)value;
		break;
	case Variant_t::KEY:
		var.key = *(Keyboard::Key*)value;
		break;
	case Variant_t::BOOL:
		var.bool_val = *(bool*)value;
		break;
	case Variant_t::REPLAY_PTR:
		var.replay_ptr = *(Replay**)value;
		break;
	default:
		break;
	}
}
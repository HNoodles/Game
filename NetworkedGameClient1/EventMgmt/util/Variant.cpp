#include "../../Objects/GameObject.h"

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
	case Variant_t::BOOL:
		var.bool_val = *(bool*)value;
	default:
		break;
	}
}
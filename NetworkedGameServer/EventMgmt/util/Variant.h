#pragma once

#include "../../Objects/GameObject.h"

enum class Variant_t {
	DOUBLE, OBJ_PTR, KEY, BOOL
};

class Variant
{
private:
	Variant_t type;
	union {
		double double_val;
		GameObject* obj_ptr;
		Keyboard::Key key;
		bool bool_val;
	} var;
public:
	Variant(Variant_t type, void* value);

	void* getVal(Variant_t variant_t)
	{
		switch (variant_t)
		{
		case Variant_t::DOUBLE:
			return &var.double_val;
		case Variant_t::OBJ_PTR:
			return &var.obj_ptr;
		case Variant_t::KEY:
			return &var.key;
		case Variant_t::BOOL:
			return &var.bool_val;
		default:
			break;
		}
	}
};


#pragma once

#include "../Objects/GameObject.h"

enum class Variant_t {
	DOUBLE, OBJ_PTR, KEY
};

class Variant
{
private:
	Variant_t type;
	union {
		double double_val;
		GameObject* obj_ptr;
		Keyboard key;
	} var;
public:
	Variant(Variant_t type, void* value);
};


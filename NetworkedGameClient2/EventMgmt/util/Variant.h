#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class GameObject;
class Replay;

enum class Variant_t {
	DOUBLE, OBJ_PTR, KEY, BOOL, REPLAY_PTR
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
		Replay* replay_ptr;
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
		case Variant_t::REPLAY_PTR:
			return &var.replay_ptr;
		default:
			return nullptr;
		}
	}
};


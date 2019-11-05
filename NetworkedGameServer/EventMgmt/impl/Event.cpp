#include "../Event.h"

void ::Event::addArg(Content_t content_t, Variant_t variant_t, void* arg)
{
	args.insert({ content_t, Variant(variant_t, arg) });
}

::Event::Event(Event_t type, double executeTime, ...) : type(type)
{
	// define an extensible argument list
	va_list args = NULL;
	// points to the next arg
	va_start(args, executeTime);
	
	switch (type)
	{
	case Event_t::CHAR_COLLISION:
		addArg(Content_t::CHARACTER_PTR, Variant_t::OBJ_PTR, va_arg(args, GameObject*));
		addArg(Content_t::OBJ_PTR, Variant_t::OBJ_PTR, va_arg(args, GameObject*));// collidable object
		break;
	case Event_t::CHAR_DEATH:
		addArg(Content_t::CHARACTER_PTR, Variant_t::OBJ_PTR, va_arg(args, GameObject*));
		break;
	case Event_t::CHAR_SPAWN:
		addArg(Content_t::CHARACTER_PTR, Variant_t::OBJ_PTR, va_arg(args, GameObject*));
		addArg(Content_t::OBJ_PTR, Variant_t::OBJ_PTR, va_arg(args, GameObject*));// spawn point
		break;
	case Event_t::USER_INPUT:
		addArg(Content_t::CHARACTER_PTR, Variant_t::OBJ_PTR, va_arg(args, GameObject*));
		addArg(Content_t::USER_INPUT, Variant_t::KEY, &va_arg(args, Keyboard));// key pressed
		break;
	case Event_t::OBJ_MOVEMENT:
		addArg(Content_t::CHARACTER_PTR, Variant_t::OBJ_PTR, va_arg(args, GameObject*));
		addArg(Content_t::X_VAL, Variant_t::DOUBLE, &va_arg(args, double));// x of to_point
		addArg(Content_t::Y_VAL, Variant_t::DOUBLE, &va_arg(args, double));// y of to_point
		break;
	default:
		break;
	}

	// release args
	va_end(args);
}

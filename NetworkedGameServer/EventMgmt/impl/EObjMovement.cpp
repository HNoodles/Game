#include "../EObjMovement.h"

EObjMovement::EObjMovement(double executeTime, GameObject* character, double x_val, double y_val)
	: Event(Event_t::OBJ_MOVEMENT, executeTime)
{
	addArg(Content_t::CHARACTER_PTR, Variant_t::OBJ_PTR, character);
	addArg(Content_t::X_VAL, Variant_t::DOUBLE, &x_val);
	addArg(Content_t::Y_VAL, Variant_t::DOUBLE, &y_val);
}

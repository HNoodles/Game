#include "../EObjMovement.h"

EObjMovement::EObjMovement(double executeTime, GameObject* character, double x_val, double y_val)
	: Event(Event_t::OBJ_MOVEMENT, executeTime)
{
	addArg(Content_t::CHARACTER_PTR, Variant_t::OBJ_PTR, character);
	addArg(Content_t::X_VAL, Variant_t::DOUBLE, &x_val);
	addArg(Content_t::Y_VAL, Variant_t::DOUBLE, &y_val);
}

string EObjMovement::toString()
{
	// E executeTime ObjID X_val Y_val
	return 
		"E " + 
		to_string(executeTime) + " " + 
		getObject()->getId() + " " + 
		to_string(getXVal()) + " " +
		to_string(getYVal());
}

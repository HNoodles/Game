#include "../EObjMovement.h"

EObjMovement::EObjMovement(double executeTime, GameObject* object, 
	double x_val, double y_val, bool positive)
	: Event(Event_t::OBJ_MOVEMENT, executeTime)
{
	addArg(Content_t::OBJ_PTR, Variant_t::OBJ_PTR, object);
	addArg(Content_t::X_VAL, Variant_t::DOUBLE, &x_val);
	addArg(Content_t::Y_VAL, Variant_t::DOUBLE, &y_val);
	addArg(Content_t::POSITIVE, Variant_t::BOOL, &positive);
}

string EObjMovement::toString()
{
	// E executeTime ObjID X_val Y_val
	return 
		"E " + 
		to_string(executeTime) + " " + 
		getObject()->getId() + " " + 
		to_string(getXVal()) + " " +
		to_string(getYVal()) + " " + 
		to_string(getPositive()) + "\n";
}

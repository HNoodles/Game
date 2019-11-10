#include "../EUserInput.h"

EUserInput::EUserInput(double executeTime, Character* character, Keyboard::Key* input)
	: Event(Event_t::USER_INPUT, executeTime)
{
	addArg(Content_t::CHARACTER_PTR, Variant_t::OBJ_PTR, &character);
	addArg(Content_t::USER_INPUT, Variant_t::KEY, &input);
}

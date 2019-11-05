#pragma once

#include <map>
#include "util/Variant.h"

#include <stdarg.h>

using namespace std;

enum class Event_t {
	CHAR_COLLISION, CHAR_DEATH, CHAR_SPAWN, USER_INPUT, OBJ_MOVEMENT
};

enum class Content_t {
	X_VAL, Y_VAL, CHARACTER_PTR, OBJ_PTR, USER_INPUT
};

class Event
{
private:
	Event_t type;
	double executeTime;
protected:
	map<Content_t, Variant> args;

	void addArg(Content_t content_t, Variant_t variant_t, void* arg);
public:
	Event(Event_t type, double executeTime);

	Event_t getType()
	{
		return type;
	}

	double getExecuteTime()
	{
		return executeTime;
	}

	Variant getArg(Content_t content_t)
	{
		return args.find(content_t)->second;
	}
};


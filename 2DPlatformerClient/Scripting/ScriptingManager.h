#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <streambuf>
#include <iostream>
#include <dukglue.h>

#include "../EventMgmt/EObjMovement.h"

using namespace std;

class ScriptingManager
{
private:
	duk_context* ctx;
	stringstream buffer;

	void initObjMovement();

	void loadScriptFromFile(const char* file_name);
public:
	ScriptingManager();

	~ScriptingManager();

	void onObjMovement(EObjMovement e);
	
};


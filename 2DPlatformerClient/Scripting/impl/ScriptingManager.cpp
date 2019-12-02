#include "../ScriptingManager.h"
#include "../../Objects/GameObject.h"
#include "../../Objects/MovingPlatform.h"
#include "../../Objects/Character.h"

ScriptingManager::ScriptingManager() : 
	ctx(duk_create_heap_default())
{
	if (!ctx) // create heap failed
	{
		exit(1);
	}

	// register classes and methods for objmovement
	initObjMovement();
}

ScriptingManager::~ScriptingManager()
{
	// destroy heap after use
	duk_destroy_heap(ctx);
}

void ScriptingManager::onObjMovement(EObjMovement e)
{
	// register global variables
	dukglue_register_global(ctx, &e, "e");
	Renderable* renderable = dynamic_cast<Renderable*>(e.getObject()->getGC(ComponentType::RENDERABLE));
	dukglue_register_global(ctx, renderable, "renderable");

	// execute script preloaded
	dukglue_peval<void>(ctx, buffer.str().c_str());

	// invalidate global variables
	dukglue_invalidate_object(ctx, &e);
	dukglue_invalidate_object(ctx, renderable);
}

void ScriptingManager::initObjMovement()
{
	loadScriptFromFile("../scripts/ObjMovement.js");

	//// register Vector2f class
	//dukglue_register_constructor<Vector2f, float, float>(ctx, "Vector2f");

	//// register component class
	//dukglue_register_constructor<ComponentType>(ctx, "ComponentType");
	//dukglue_register_constructor<GenericComponent, GameObject*>(ctx, "GenericComponent");
	//dukglue_register_constructor<Renderable, GameObject*, ::Shape, ::Color, Vector2f, Vector2f>(ctx, "Renderable");
	//dukglue_register_property(ctx, &Renderable::getShape, nullptr, "shape");

	//// register game object class
	//dukglue_register_constructor<GameObject, string, EventManager*>(ctx, "GameObject");
	//dukglue_register_property(ctx, &GameObject::getId, nullptr, "id");
	//dukglue_register_method(ctx, &GameObject::getGC, "getGC");

	//dukglue_register_constructor<MovingPlatform, string, EventManager*, 
	//	::Shape, ::Color, Vector2f, Vector2f, 
	//	Vector2f, Timeline&, ::Move, float, float>(ctx, "MovingPlatform");
	//dukglue_register_property(ctx, &MovingPlatform::getHeadingPositive, &MovingPlatform::setHeadingPositive, "headingPositive");

	//dukglue_register_constructor<Character, string, EventManager*,
	//	::Shape, ::Color, Vector2f, Vector2f,
	//	Vector2f, Timeline&, vector<SpawnPoint*>*, Vector2f*, vector<SideBoundary*>*>(ctx, "Character");
	//dukglue_register_property(ctx, &Character::getRenderOffset, &Character::setRenderOffset, "renderOffset");

	// register EObjmovement class
	dukglue_register_constructor
		<EObjMovement, double, GameObject*, double, double,
		bool, double, double>(ctx, "EObjMovement");
	dukglue_register_property(ctx, &EObjMovement::getObject, nullptr, "object");
	dukglue_register_property(ctx, &EObjMovement::getXVal, nullptr, "xVal");
	dukglue_register_property(ctx, &EObjMovement::getYVal, nullptr, "yVal");
	dukglue_register_property(ctx, &EObjMovement::getOffsetX, nullptr, "offsetX");
	dukglue_register_property(ctx, &EObjMovement::getOffsetY, nullptr, "offsetY");
	dukglue_register_property(ctx, &EObjMovement::getPositive, nullptr, "positive");

}

void ScriptingManager::loadScriptFromFile(const char* file_name)
{
	ifstream t(file_name);
	buffer.str("");// empty buffer before load
	buffer << t.rdbuf();
}

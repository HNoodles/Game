#pragma once
#include <map>
#include "../Components/GenericComponent.h"

using namespace std;

class GameObject
{
private:
	map<ComponentType, GenericComponent*> gcs;
public:
	GameObject() {};

	GameObject(map<ComponentType, GenericComponent*> gcs) { this->gcs = gcs; }

	/*map<ComponentType, GenericComponent*>& getGCS()
	{
		return gcs;
	}*/

	GenericComponent* getGC(ComponentType type)
	{
		auto pair = gcs.find(type);
		return pair != gcs.end() ? pair->second : nullptr;
	}

	void addGC(ComponentType type, GenericComponent* component)
	{
		gcs.insert({ type, component });
	}
};
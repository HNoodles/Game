#pragma once
#include <map>
#include "../Components/GenericComponent.h"
#include "../EventMgmt/EventManager.h"

using namespace std;

class GameObject
{
private:
	string id;
	map<ComponentType, GenericComponent*> gcs;
	EventManager* manager;
public:
	GameObject(string id, EventManager* manager) : id(id), manager(manager) {};

	virtual ~GameObject()
	{
		for (auto pair : gcs) 
		{
			delete pair.second;
		}
	}

	GenericComponent* getGC(ComponentType type)
	{
		auto pair = gcs.find(type);
		return pair != gcs.end() ? pair->second : nullptr;
	}

	void addGC(ComponentType type, GenericComponent* component)
	{
		gcs.insert({ type, component });
	}

	string getId() const
	{
		return id;
	}

	EventManager* getEM()
	{
		return manager;
	}
};
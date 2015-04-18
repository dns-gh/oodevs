#include "EngineObject.h"

unsigned int EngineObject::uniqueid_ = 0;

EngineObject::EngineObject()
{
	id_ = uniqueid_++;
}

unsigned int EngineObject::GetID()
{
	return id_;
}
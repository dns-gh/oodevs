#ifndef _ENGINEOBJECT_H
#define _ENGINEOBJECT_H

class EngineObject
{
public:
	EngineObject();
	unsigned int GetID() const;

private:
	static unsigned int uniqueid_;
	unsigned id_; // unique id given when created
};

#endif // EngineObject.h
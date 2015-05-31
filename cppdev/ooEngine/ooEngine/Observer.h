#ifndef _OBSERVER_H_
#define _OBSERVER_H_

#include "EngineObject.h"

#include <string>

template< typename T >
class Observer
{
public:
    virtual ~Observer(){};
    virtual void OnNotify( const T& element, unsigned int eventType ) = 0;
};

#endif
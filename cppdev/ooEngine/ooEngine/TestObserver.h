#ifndef _TEST_OBSERVER_H_
#define _TEST_OBSERVER_H_

#include "LogTools.h"
#include "Observer.h"
#include "SceneObject.h"

class TestObserver : public Observer< SceneObject >
{
public:
    virtual void Observer< SceneObject >::OnNotify( const SceneObject& element, unsigned int /*event*/ )
    {
        // Init of a logger
        auto logger = std::make_shared< LogTools >( tools::GetModulePath( ) );
        logger->RegisterLog( FILE_INFOS, "ooEngine.log" );
        logger->OOLOG( FILE_INFOS ) << OOSTREAM( LOG_MESSAGE, "something happened! " << element.IsVisible() );
    }
};

#endif
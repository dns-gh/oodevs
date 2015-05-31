#ifndef _SUBJECT_H_
#define _SUBJECT_H_

#include "EngineObject.h"
#include "Observer.h"

#include <memory>
#include <vector>
#include <algorithm>
#include <string>

template< typename T >
class Subject
{
    typedef std::shared_ptr< Observer< T > > Ptr;

public:
    virtual ~Subject(){};
    void AddObserver( const Ptr& observer ) {
        observers_.push_back( observer );
    }

    void RemoveObserver( const Ptr& toremove )
    {
        auto it = std::find_if( observers_.begin(), observers_.end(), [&]( const Ptr& observer ){
            return toremove.get() == observer.get();
        } );
        if( it != observers_.end() )
            observers_.erase( it );
    }

protected:
    void Notify( const T& object, unsigned int eventType )
    {
        for( auto it : observers_ )
            it->OnNotify( object, eventType );
    }

private:
    std::vector< Ptr > observers_;
};
#endif
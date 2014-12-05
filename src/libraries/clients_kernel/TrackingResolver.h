// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef TOOLS_TRACKING_RESOLVER_H
#define TOOLS_TRACKING_RESOLVER_H

#include "Controllers.h"
#include <tools/Resolver.h>
#include <tools/ElementObserver_ABC.h>
#include <boost/noncopyable.hpp>

namespace tools
{
// =============================================================================
/** @class  TrackingResolver
    @brief  A resolver which automatically removes destroyed elements
*/
// Created: MCO 2014-11-27
// =============================================================================
template< typename T, typename Base = T >
class TrackingResolver : public Resolver< T >
                       , public Observer_ABC
                       , public ElementObserver_ABC< Base >
                       , private boost::noncopyable
{
public:
    explicit TrackingResolver( kernel::Controllers& controllers )
        : controllers_( controllers )
    {
        controllers_.Register( *this );
    }

    virtual ~TrackingResolver()
    {
        controllers_.Unregister( *this );
    }

    void Add( T& t )
    {
        Register( t.GetId(), t );
    }

private:
    virtual void NotifyDeleted( const Base& t )
    {
        Remove( t.GetId() );
    }

private:
    kernel::Controllers& controllers_;
};

}

#endif // TOOLS_TRACKING_RESOLVER_H

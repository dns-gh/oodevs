// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __dispatcher_Observer_h
#define __dispatcher_Observer_h

#include "Observable.h"
#include <boost/noncopyable.hpp>

namespace dispatcher
{
    template< typename T > class Observable;

// =============================================================================
/** @class  Observer
    @brief  Observer
*/
// Created: SLI 2010-09-01
// =============================================================================
template< typename T >
class Observer : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Observer( Observable< T >& observable )
        : observable_( observable )
    {
        observable_.Register( *this );
    }
    virtual ~Observer()
    {
        observable_.Unregister( *this );
    }
    virtual void Notify( const T& message ) = 0;
    //@}

private:
    //! @name Member data
    //@{
    Observable< T >& observable_;
    //@}
};

}

#endif // __dispatcher_Observer_h
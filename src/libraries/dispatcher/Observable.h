// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __dispatcher_Observable_h_
#define __dispatcher_Observable_h_

#include "Observer.h"
#include <boost/bind.hpp>
#include <boost/noncopyable.hpp>
#include <vector>
#include <algorithm>

namespace dispatcher
{
    template< typename T > class Observer;

// =============================================================================
/** @class  Observable
    @brief  Observable
*/
// Created: SLI 2010-08-18
// =============================================================================
template< typename T >
class Observable : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Observable() {}
    virtual ~Observable() {}
    //@}

    //! @name Operations
    //@{
    virtual void Register( Observer< T >& observer )
    {
        observers_.push_back( &observer );
    }
    virtual void Unregister( Observer< T >& observer )
    {
        observers_.erase( std::remove( observers_.begin(), observers_.end(), &observer ), observers_.end() );
    }
    virtual void Notify( const T& message )
    {
        std::for_each( observers_.begin(), observers_.end(), boost::bind( &Observer< T >::Notify, _1, boost::cref( message ) ) );
    }
    //@}

private:
    //! @name Member data
    //@{
    std::vector< Observer< T >* > observers_;
    //@}
};

}

#endif // __dispatcher_Observable_h_

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef dispatcher_OrderResolver_ABC_h
#define dispatcher_OrderResolver_ABC_h

#include <boost/noncopyable.hpp>

namespace sword
{
    class ClientToSim;
}

namespace dispatcher
{
// =============================================================================
/** @class  OrderResolver_ABC
    @brief  Order resolver declaration
*/
// Created: LGY 2011-09-05
// =============================================================================
class OrderResolver_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             OrderResolver_ABC() {}
    virtual ~OrderResolver_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Resolve( const sword::ClientToSim& message ) = 0;
    //@}
};

}

#endif // dispatcher_OrderResolver_ABC_h

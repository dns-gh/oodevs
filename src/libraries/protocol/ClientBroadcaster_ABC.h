// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Masagroup
//
// *****************************************************************************

#ifndef __ClientBroadcaster_ABC_h_
#define __ClientBroadcaster_ABC_h_

#include <boost/noncopyable.hpp>
#include <string>

namespace sword
{
    class SimToClient;
}

namespace dispatcher
{
// =============================================================================
/** @class  ClientBroadcaster_ABC
    @brief  ClientBroadcaster_ABC
*/
// Created: MCO 2011-11-07
// =============================================================================
class ClientBroadcaster_ABC : boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ClientBroadcaster_ABC() {}
    virtual ~ClientBroadcaster_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Activate( const std::string& link ) = 0;
    virtual void Deactivate( const std::string& link ) = 0;

    virtual void Broadcast( const sword::SimToClient& message ) = 0;
    //@}
};

}

#endif // __ClientBroadcaster_ABC_h_

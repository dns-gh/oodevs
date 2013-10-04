// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Plugin_ABC_h_
#define __Plugin_ABC_h_

#include "MessageHandler_ABC.h"
#include <string>

namespace dispatcher
{
    class Profile_ABC;
    class ClientPublisher_ABC;
    class Services;

// =============================================================================
/** @class  Plugin_ABC
    @brief  Plugin declaration
*/
// Created: AGE 2007-08-24
// =============================================================================
class Plugin_ABC : public MessageHandler_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Plugin_ABC() {}
    virtual ~Plugin_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void NotifyClientAuthenticated( dispatcher::ClientPublisher_ABC& /*client*/, const std::string& /*link*/,
                                            dispatcher::Profile_ABC& /*profile*/, bool /*uncounted*/ )
    {}
    virtual void NotifyClientLeft( dispatcher::ClientPublisher_ABC& /*client*/, const std::string& /*link*/ )
    {}

    virtual void Register( dispatcher::Services& ) {}
    virtual void Update() {}
    virtual void Close() {}

    // Provides a way to filter out messages from the simulation so that they
    // are not sent to clients. Returning true means not propagating a message
    // through the message handler chain. Therefore MessageHandler_ABC::Receive
    // will not be called for this plugin.
    virtual bool Filter( const sword::SimToClient& /*message*/ ) const
    {
        return false;
    }
    //@}
};

}

#endif // __Plugin_ABC_h_

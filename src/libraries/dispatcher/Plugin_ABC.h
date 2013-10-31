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
#include <boost/noncopyable.hpp>
#include <string>

namespace sword
{
    class ClientToSim;
}

namespace dispatcher
{
    class Profile_ABC;
    class ClientPublisher_ABC;
    class Services;

// Similar to ClientPublisher_ABC, but can rewrite messages before sending
// them, mostly to add routing annotations.
class RewritingPublisher_ABC : private boost::noncopyable
{
public:
             RewritingPublisher_ABC() {}
    virtual ~RewritingPublisher_ABC() {}

    virtual void Send( sword::SimToClient& message ) = 0;
};

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
    virtual void NotifyClientLeft( dispatcher::ClientPublisher_ABC& /*client*/, const std::string& /*link*/,
                                   bool /*uncounted*/ )
    {}

    virtual void Register( dispatcher::Services& ) {}
    virtual void Update() {}
    virtual void Close() {}

    // Returns true if the message was handled and a response emitted. It is
    // the handler responsibility to ensure a response is sent, even upon
    // processing error. Handled messages are not forwarded to other plugins.
    virtual bool HandleClientToSim( const sword::ClientToSim&, RewritingPublisher_ABC&,
            ClientPublisher_ABC& )
    {
        return false;
    }
    //@}
};

}

#endif // __Plugin_ABC_h_

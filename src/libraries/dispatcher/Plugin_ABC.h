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
#include <cstdint>
#include <string>

namespace sword
{
    class ClientToReplay;
    class ClientToSim;
    class ReplayToClient;
    class SimToClient;
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

    virtual const std::string& GetLink() const = 0;
    virtual void Send( sword::ReplayToClient& message ) = 0;
    virtual void Send( sword::SimToClient& message ) = 0;
};

// A RewritingPublisher_ABC rewriting output messages client identifier and
// context number.
class UnicastPublisher : public RewritingPublisher_ABC
{
public:
    UnicastPublisher( ClientPublisher_ABC& publisher, const std::string& link,
            int32_t clientId, int32_t context );

    virtual const std::string& GetLink() const;
    virtual void Send( sword::ReplayToClient& message );
    virtual void Send( sword::SimToClient& message );

private:
    ClientPublisher_ABC& publisher_;
    const std::string link_;
    const int32_t clientId_;
    const int32_t context_;
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
    virtual void NotifySimulationLeft() {}

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
    virtual bool HandleClientToReplay( const sword::ClientToReplay&,
            RewritingPublisher_ABC&, ClientPublisher_ABC& )
    {
        return false;
    }
    //@}
};

}

#endif // __Plugin_ABC_h_

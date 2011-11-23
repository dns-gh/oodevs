// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Client_h_
#define __Client_h_

#include "protocol/ClientPublisher_ABC.h"

namespace tools
{
    class MessageSender_ABC;
    class Message;
}

namespace dispatcher
{
    class ClientBroadcaster_ABC;

// =============================================================================
/** @class  Client
    @brief  Client
*/
// Created: NLD 2006-09-19
// =============================================================================
class Client : public ClientPublisher_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Client( tools::MessageSender_ABC& sender, ClientBroadcaster_ABC& broadcaster, const std::string& endpoint );
    virtual ~Client();
    //@}

    //! @name Operations
    //@{
    void Activate();
    void Deactivate();

    virtual void Send( const sword::AuthenticationToClient& msg );
    virtual void Send( const sword::SimToClient& msg );
    virtual void Send( const sword::ReplayToClient& msg );
    virtual void Send( const sword::AarToClient& msg );
    virtual void Send( const sword::MessengerToClient& msg );
    virtual void Send( const sword::DispatcherToClient& msg );

    void Send( unsigned long tag, tools::Message& message ) const;

    bool HasAnsweredSinceLastTick();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Client( const Client& );            //!< Copy constructor
    Client& operator=( const Client& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    tools::MessageSender_ABC& sender_;
    ClientBroadcaster_ABC& broadcaster_;
    std::string endpoint_;
    //@}
};

}

#endif // __Client_h_

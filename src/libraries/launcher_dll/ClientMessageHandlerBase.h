// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef launcher_ClientMessageHandlerBase_h
#define launcher_ClientMessageHandlerBase_h

#include "MessageHandler_ABC.h"
#include "LauncherPublisher.h"
#include "protocol/ClientSenders.h"
#include "protocol/LauncherSenders.h"
#include "protocol/AuthenticationSenders.h"
#include "protocol/DispatcherSenders.h"
#include <boost/shared_ptr.hpp>

namespace launcher
{
    class LauncherPublisher;

// =============================================================================
/** @class  ClientMessageHandlerBase
    @brief  Client message handler base
*/
// Created: LGY 2011-05-18
// =============================================================================
class ClientMessageHandlerBase : public MessageHandler_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ClientMessageHandlerBase( boost::shared_ptr< LauncherPublisher >, const std::string& exercise, const std::string& session );
    virtual ~ClientMessageHandlerBase();
    //@}

    //! @name Operations
    //@{
    virtual bool OnReceiveMessage( const sword::SimToClient& message );
    virtual bool OnReceiveMessage( const sword::MessengerToClient& message );
    virtual bool OnReceiveMessage( const sword::AuthenticationToClient& message );
    virtual bool OnReceiveMessage( const sword::DispatcherToClient& message );
    //@}

protected:
    //! @name operations
    //@{
    template< typename T >
    void Send( T& message )
    {
        message().set_exercise( exercise_ );
        message().set_session( session_ );
        if ( publisher_.get() )
            message.Send( *publisher_ );
    }
    template< typename T >
    void SendSimply( T& message )
    {
        if ( publisher_.get() )
            message.Send( *publisher_ );
    }
    //@}

private:
    //! @name Member data
    //@{
    boost::shared_ptr< LauncherPublisher > publisher_;
    const std::string exercise_;
    const std::string session_;
    //@}
};

}

#endif // launcher_ClientMessageHandlerBase_h_

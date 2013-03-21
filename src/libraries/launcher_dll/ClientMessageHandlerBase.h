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
#include <tools/Path.h>

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
             ClientMessageHandlerBase( LauncherPublisher& publisher, const tools::Path& exercise, const tools::Path& session );
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
        message().set_exercise( exercise_.ToUTF8() );
        message().set_session( session_.ToUTF8() );
        message.Send( publisher_ );
    }
    //@}

private:
    //! @name Member data
    //@{
    LauncherPublisher& publisher_;
    const tools::Path exercise_;
    const tools::Path session_;
    //@}
};

}

#endif // launcher_ClientMessageHandlerBase_h_

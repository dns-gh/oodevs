// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef launcher_NotificationMessageHandler_h
#define launcher_NotificationMessageHandler_h

#include "ClientMessageHandlerBase.h"

namespace launcher
{
// =============================================================================
/** @class  NotificationMessageHandler
    @brief  Notification message handler
*/
// Created: LGY 2011-05-18
// =============================================================================
class NotificationMessageHandler : public ClientMessageHandlerBase
{
public:
    //! @name Constructors/Destructor
    //@{
             NotificationMessageHandler( LauncherPublisher& publisher, const std::string& exercise, const std::string& session );
    virtual ~NotificationMessageHandler();
    //@}

    //! @name Operations
    //@{
    virtual bool OnReceiveMessage( const sword::SimToClient& message );
    virtual bool OnReceiveMessage( const sword::AuthenticationToClient& message );
    //@}
};

}

#endif // launcher_NotificationMessageHandler_h

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __LAUNCHER_CONNECTEDPROFILESMESSAGEHANDLER_H__
#define __LAUNCHER_CONNECTEDPROFILESMESSAGEHANDLER_H__

#include "ClientMessageHandlerBase.h"

namespace launcher
{
// =============================================================================
/** @class  ConnectedProfilesMessageHandler
    @brief  ConnectedProfilesMessageHandler
*/
// Created: AHC 2011-05-20
// =============================================================================
class ConnectedProfilesMessageHandler : public ClientMessageHandlerBase
{
public:
    //! @name Constructors/Destructor
    //@{
    ConnectedProfilesMessageHandler( LauncherPublisher& publisher, const std::string& exercise, const std::string& session );
    ~ConnectedProfilesMessageHandler();
    //@}

    //! @name Operations
    //@{
    bool OnReceiveMessage( const sword::AuthenticationToClient& message );
    //@}
};

}

#endif // __LAUNCHER_CONNECTEDPROFILESMESSAGEHANDLER_H__

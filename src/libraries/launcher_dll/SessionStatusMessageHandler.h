// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __SessionStatusMessageHandler_h_
#define __SessionStatusMessageHandler_h_

#include "ClientMessageHandlerBase.h"

namespace launcher
{
// =============================================================================
/** @class  SessionStatusMessageHandler
    @brief  Session status message handler
*/
// Created: LGY 2011-11-17
// =============================================================================
class SessionStatusMessageHandler : public ClientMessageHandlerBase
{
public:
    //! @name Constructors/Destructor
    //@{
             SessionStatusMessageHandler( LauncherPublisher& publisher, const std::string& exercise, const std::string& session );
    virtual ~SessionStatusMessageHandler();
    //@}

    //! @name Operations
    //@{
    virtual bool OnReceiveMessage( const sword::SimToClient& message );
    //@}
};

}

#endif // __SessionStatusMessageHandler_h_

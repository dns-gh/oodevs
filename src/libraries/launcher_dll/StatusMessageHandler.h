// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef launcher_StatusMessageHandler_h
#define launcher_StatusMessageHandler_h

#include "ClientMessageHandlerBase.h"

namespace launcher
{
// =============================================================================
/** @class  StatusMessageHandler
    @brief  Status message handler
*/
// Created: LGY 2011-05-18
// =============================================================================
class StatusMessageHandler : public ClientMessageHandlerBase
{
public:
    //! @name Constructors/Destructor
    //@{
             StatusMessageHandler( LauncherPublisher& publisher, const std::string& exercise, const std::string& session );
    virtual ~StatusMessageHandler();
    //@}

    //! @name Operations
    //@{
    virtual bool OnReceiveMessage( const sword::SimToClient& message );
    //@}
};

}

#endif // launcher_StatusMessageHandler_h

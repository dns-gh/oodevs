// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef launcher_PauseResumeMessageHandler_h
#define launcher_PauseResumeMessageHandler_h

#include "ClientMessageHandlerBase.h"

namespace launcher
{
// =============================================================================
/** @class  PauseResumeMessageHandler
    @brief  Pause Resume message handler
*/
// Created: LGY 2011-05-18
// =============================================================================
class PauseResumeMessageHandler : public ClientMessageHandlerBase
{
public:
    //! @name Constructors/Destructor
    //@{
             PauseResumeMessageHandler( LauncherPublisher& publisher, const std::string& exercise, const std::string& session );
    virtual ~PauseResumeMessageHandler();
    //@}

    //! @name Operations
    //@{
    virtual bool OnReceiveMessage( const sword::SimToClient& message );
    //@}
};

}

#endif // launcher_PauseResumeMessageHandler_h

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef launcher_CheckpointMessageHandler_h
#define launcher_CheckpointMessageHandler_h

#include "ClientMessageHandlerBase.h"

namespace launcher
{
// =============================================================================
/** @class  CheckpointMessageHandler
    @brief  Checkpoint message handler
*/
// Created: LGY 2011-05-18
// =============================================================================
class CheckpointMessageHandler : public ClientMessageHandlerBase
{
public:
    //! @name Constructors/Destructor
    //@{
             CheckpointMessageHandler( LauncherPublisher& publisher, const std::string& exercise, const std::string& session );
    virtual ~CheckpointMessageHandler();
    //@}

    //! @name Operations
    //@{
    virtual bool OnReceiveMessage( const sword::SimToClient& message );
    //@}
};

}

#endif // launcher_CheckpointMessageHandler_h

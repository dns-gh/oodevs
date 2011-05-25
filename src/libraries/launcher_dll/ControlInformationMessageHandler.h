// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************


#ifndef __LAUNCHER_CONTROLINFORMATIONMESSAGEHANDLER_H__
#define __LAUNCHER_CONTROLINFORMATIONMESSAGEHANDLER_H__

#include "ClientMessageHandlerBase.h"

namespace launcher
{
// =============================================================================
/** @class  ControlInformationMessageHandler
    @brief  ControlInformationMessageHandler
*/
// Created: AHC 2011-05-20
// =============================================================================
class ControlInformationMessageHandler : public ClientMessageHandlerBase
{
public:
    //! @name Constructors/Destructor
    //@{
             ControlInformationMessageHandler( LauncherPublisher& publisher, const std::string& exercise, const std::string& session );
    virtual ~ControlInformationMessageHandler();
    //@}

    //! @name Operations
    //@{
    virtual bool OnReceiveMessage( const sword::SimToClient& message );
    //@}
};

}

#endif // __LAUNCHER_CONTROLINFORMATIONMESSAGEHANDLER_H__

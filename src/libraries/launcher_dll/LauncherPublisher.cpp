// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "launcher_dll_pch.h"
#include "LauncherPublisher.h"
#include "protocol/LauncherSenders.h"
#include "tools/MessageSender_ABC.h"
#include "MT_Tools/MT_Logger.h"
#include "MT_Tools/MT_FormatString.h"

using namespace launcher;

// -----------------------------------------------------------------------------
// Name: LauncherPublisher constructor
// Created: SBO 2010-09-29
// -----------------------------------------------------------------------------
LauncherPublisher::LauncherPublisher( tools::MessageSender_ABC& sender, const std::string& endpoint )
    : sender_  ( sender )
    , endpoint_( endpoint )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LauncherPublisher destructor
// Created: SBO 2010-09-29
// -----------------------------------------------------------------------------
LauncherPublisher::~LauncherPublisher()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LauncherPublisher::Send
// Created: SBO 2010-09-29
// -----------------------------------------------------------------------------
void LauncherPublisher::Send( const sword::LauncherToAdmin& message )
{
	try
	{
		sender_.Send( endpoint_, message );
	}
    catch( std::exception& e )
    {
        std::string content = e.what();
        MT_LOG_ERROR_MSG( MT_FormatString( "Sending message to Admin failed, error code = %d", content) );
    }
}

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "frontend_pch.h"
#include "LauncherPublisher.h"
#include "protocol/LauncherSenders.h"
#include "tools/MessageSender_ABC.h"

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: LauncherPublisher constructor
// Created: SBO 2010-09-29
// -----------------------------------------------------------------------------
LauncherPublisher::LauncherPublisher( tools::MessageSender_ABC& sender, const std::string& endpoint )
    : sender_( sender )
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
void LauncherPublisher::Send( const MsgsAdminToLauncher::MsgAdminToLauncher& message )
{
    sender_.Send( endpoint_, message );
}

// -----------------------------------------------------------------------------
// Name: LauncherPublisher::Send
// Created: SBO 2010-09-29
// -----------------------------------------------------------------------------
void LauncherPublisher::Send( const MsgsLauncherToAdmin::MsgLauncherToAdmin& message )
{
    sender_.Send( endpoint_, message );
}

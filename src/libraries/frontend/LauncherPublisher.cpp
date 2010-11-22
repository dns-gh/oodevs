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
LauncherPublisher::LauncherPublisher( tools::MessageSender_ABC& sender )
    : sender_( sender )
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
// Name: LauncherPublisher::IsSame
// Created: SBO 2010-11-22
// -----------------------------------------------------------------------------
bool LauncherPublisher::IsSame( const std::string& endpoint ) const
{
    return endpoint_ == endpoint; // $$$$ SBO 2010-11-22: find something better, resolve host
}

// -----------------------------------------------------------------------------
// Name: LauncherPublisher::SetHost
// Created: SBO 2010-11-22
// -----------------------------------------------------------------------------
void LauncherPublisher::SetHost( const std::string& endpoint )
{
    endpoint_ = endpoint;
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

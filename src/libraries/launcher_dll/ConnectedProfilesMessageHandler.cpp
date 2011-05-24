// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "launcher_dll_pch.h"
#include "ConnectedProfilesMessageHandler.h"
#include "protocol/AuthenticationSenders.h"
#include "protocol/LauncherSenders.h"

using namespace launcher;

// -----------------------------------------------------------------------------
// Name: ConnectedProfilesMessageHandler constructor
// Created: AHC 2011-05-20
// -----------------------------------------------------------------------------
ConnectedProfilesMessageHandler::ConnectedProfilesMessageHandler( LauncherPublisher& publisher, const std::string& exercise, const std::string& session )
    : ClientMessageHandlerBase( publisher, exercise, session )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ConnectedProfilesMessageHandler destructor
// Created: AHC 2011-05-20
// -----------------------------------------------------------------------------
ConnectedProfilesMessageHandler::~ConnectedProfilesMessageHandler()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ConnectedProfilesMessageHandler::OnReceiveMessage
// Created: AHC 2011-05-20
// -----------------------------------------------------------------------------
bool ConnectedProfilesMessageHandler::OnReceiveMessage( const sword::AuthenticationToClient& message )
{
    bool retval = false;
    if( message.message().has_connected_profile_list() )
    {
        const sword::ConnectedProfileList& profiles =  message.message().connected_profile_list();
        launcher::ConnectedProfileListResponse response;
        response().set_error_code( sword::ConnectedProfileListResponse::success );
        response().mutable_profile()->MergeFrom( profiles.elem() );
        Send( response );
    }
    return retval;
}

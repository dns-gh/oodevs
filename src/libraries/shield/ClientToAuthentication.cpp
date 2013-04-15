// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "ClientToAuthentication.h"
#include "ClientTools.h"

using namespace shield;

// -----------------------------------------------------------------------------
// Name: ClientToAuthentication::Convert
// Created: SBO 2010-11-24
// -----------------------------------------------------------------------------
void ClientToAuthentication::Convert( const MsgsClientToAuthentication::MsgAuthenticationRequest& from, sword::AuthenticationRequest* to )
{
    CONVERT( login );
    CONVERT( password );
    CONVERT_VERSION( version );
}

// -----------------------------------------------------------------------------
// Name: ClientToAuthentication::Convert
// Created: SBO 2010-11-24
// -----------------------------------------------------------------------------
void ClientToAuthentication::Convert( const MsgsClientToAuthentication::MsgProfileCreationRequest& from, sword::ProfileCreationRequest* to )
{
    CONVERT_CB( profile, ConvertProfile );
}

// -----------------------------------------------------------------------------
// Name: ClientToAuthentication::Convert
// Created: SBO 2010-11-24
// -----------------------------------------------------------------------------
void ClientToAuthentication::Convert( const MsgsClientToAuthentication::MsgProfileUpdateRequest& from, sword::ProfileUpdateRequest* to )
{
    CONVERT( login );
    CONVERT_CB( profile, ConvertProfile );
}

// -----------------------------------------------------------------------------
// Name: ClientToAuthentication::Convert
// Created: SBO 2010-11-24
// -----------------------------------------------------------------------------
void ClientToAuthentication::Convert( const MsgsClientToAuthentication::MsgProfileDestructionRequest& from, sword::ProfileDestructionRequest* to )
{
    CONVERT( login );
}

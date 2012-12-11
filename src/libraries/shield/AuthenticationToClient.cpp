// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "AuthenticationToClient.h"
#include "SimulationTools.h"
#include <tools/Exception.h>

using namespace shield;

// -----------------------------------------------------------------------------
// Name: AuthenticationToClient::Convert
// Created: MCO 2010-11-08
// -----------------------------------------------------------------------------
void AuthenticationToClient::Convert( const sword::AuthenticationResponse& from, MsgsAuthenticationToClient::MsgAuthenticationResponse* to )
{
    CONVERT_ENUM( error_code, ( sword::AuthenticationResponse::success, MsgsAuthenticationToClient::MsgAuthenticationResponse::success )
                              ( sword::AuthenticationResponse::invalid_login, MsgsAuthenticationToClient::MsgAuthenticationResponse::invalid_login )
                              ( sword::AuthenticationResponse::too_many_connections, MsgsAuthenticationToClient::MsgAuthenticationResponse::too_many_connections )
                              ( sword::AuthenticationResponse::mismatched_protocol_version, MsgsAuthenticationToClient::MsgAuthenticationResponse::mismatched_protocol_version ) );
    CONVERT_CB( profile, ConvertProfile );
    CONVERT_LIST( profiles, elem, ConvertProfileDescription );
    CONVERT_VERSION( server_version );
    CONVERT_DATE( restart_date_time );
    CONVERT( terrain_name );
}

// -----------------------------------------------------------------------------
// Name: AuthenticationToClient::Convert
// Created: MCO 2010-11-08
// -----------------------------------------------------------------------------
void AuthenticationToClient::Convert( const sword::ProfileCreation& from, MsgsAuthenticationToClient::MsgProfileCreation* to )
{
    CONVERT_CB( profile, ConvertProfile );
}

// -----------------------------------------------------------------------------
// Name: AuthenticationToClient::Convert
// Created: MCO 2010-11-08
// -----------------------------------------------------------------------------
void AuthenticationToClient::Convert( const sword::ProfileCreationRequestAck& from, MsgsAuthenticationToClient::MsgProfileCreationRequestAck* to )
{
    CONVERT( login );
    CONVERT_ENUM( error_code, ( sword::ProfileCreationRequestAck::success, MsgsAuthenticationToClient::MsgProfileCreationRequestAck::success )
                              ( sword::ProfileCreationRequestAck::invalid_login, MsgsAuthenticationToClient::MsgProfileCreationRequestAck::invalid_login )
                              ( sword::ProfileCreationRequestAck::invalid_password, MsgsAuthenticationToClient::MsgProfileCreationRequestAck::invalid_password )
                              ( sword::ProfileCreationRequestAck::duplicate_login, MsgsAuthenticationToClient::MsgProfileCreationRequestAck::duplicate_login ) );
}

// -----------------------------------------------------------------------------
// Name: AuthenticationToClient::Convert
// Created: MCO 2010-11-08
// -----------------------------------------------------------------------------
void AuthenticationToClient::Convert( const sword::ProfileUpdate& from, MsgsAuthenticationToClient::MsgProfileUpdate* to )
{
    CONVERT( login );
    CONVERT_CB( profile, ConvertProfile );
}

// -----------------------------------------------------------------------------
// Name: AuthenticationToClient::Convert
// Created: MCO 2010-11-08
// -----------------------------------------------------------------------------
void AuthenticationToClient::Convert( const sword::ProfileUpdateRequestAck& from, MsgsAuthenticationToClient::MsgProfileUpdateRequestAck* to )
{
    CONVERT( login );
    CONVERT_ENUM( error_code, ( sword::ProfileUpdateRequestAck::success, MsgsAuthenticationToClient::MsgProfileUpdateRequestAck::success )
                              ( sword::ProfileUpdateRequestAck::invalid_profile, MsgsAuthenticationToClient::MsgProfileUpdateRequestAck::invalid_profile )
                              ( sword::ProfileUpdateRequestAck::invalid_login, MsgsAuthenticationToClient::MsgProfileUpdateRequestAck::invalid_login )
                              ( sword::ProfileUpdateRequestAck::invalid_password, MsgsAuthenticationToClient::MsgProfileUpdateRequestAck::invalid_password )
                              ( sword::ProfileUpdateRequestAck::duplicate_login, MsgsAuthenticationToClient::MsgProfileUpdateRequestAck::duplicate_login ) );
}

// -----------------------------------------------------------------------------
// Name: AuthenticationToClient::Convert
// Created: MCO 2010-11-08
// -----------------------------------------------------------------------------
void AuthenticationToClient::Convert( const sword::ProfileDestruction& from, MsgsAuthenticationToClient::MsgProfileDestruction* to )
{
    CONVERT( login );
}

// -----------------------------------------------------------------------------
// Name: AuthenticationToClient::Convert
// Created: MCO 2010-11-08
// -----------------------------------------------------------------------------
void AuthenticationToClient::Convert( const sword::ProfileDestructionRequestAck& from, MsgsAuthenticationToClient::MsgProfileDestructionRequestAck* to )
{
    CONVERT_ENUM( error_code, ( sword::ProfileDestructionRequestAck::success, MsgsAuthenticationToClient::MsgProfileDestructionRequestAck::success )
                              ( sword::ProfileDestructionRequestAck::invalid_profile, MsgsAuthenticationToClient::MsgProfileDestructionRequestAck::invalid_profile )
                              ( sword::ProfileDestructionRequestAck::failure, MsgsAuthenticationToClient::MsgProfileDestructionRequestAck::failure ) );
    CONVERT( login );
}

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "RightsHelper.h"

namespace protocol
{

#define RETURN_CONTROL_ERROR(in, name, out)                                     \
    if ( in.message().has_##name() )                                        \
    {                                                                           \
        out.mutable_message()->mutable_##name##_ack()                         \
            ->set_error_code( sword::ControlAck::error_forbidden );             \
        return true;                                                            \
    }

bool GetForbiddenError( const sword::ClientToSim& in, sword::SimToClient& out)
{
    RETURN_CONTROL_ERROR( in, control_pause, out );
    RETURN_CONTROL_ERROR( in, control_resume, out );
    RETURN_CONTROL_ERROR( in, control_stop, out );
    return false;
}

namespace
{

std::string GetLogin( const sword::ClientToAuthentication& msg )
{
    auto m = msg.message();
    if( m.has_profile_creation_request() )
        return m.profile_creation_request().profile().login();
    if( m.has_profile_update_request() )
        return m.profile_update_request().login();
    if( m.has_profile_destruction_request() )
        return m.profile_destruction_request().login();
    return "unknown";
}

}  // namespace

#define RETURN_AUTH_ERROR(in, name, ackname, out)                               \
    if ( in.message().has_##name() )                                        \
    {                                                                           \
        auto ack = out.mutable_message()->mutable_##name##_ack();             \
        ack->set_error_code( sword::ackname::forbidden );                   \
        ack->set_login( GetLogin( in ));                                        \
        return true;                                                            \
    }

bool GetForbiddenError( const sword::ClientToAuthentication& in,
        sword::AuthenticationToClient& out)
{
    RETURN_AUTH_ERROR( in, profile_creation_request, ProfileCreationRequestAck, out );
    RETURN_AUTH_ERROR( in, profile_update_request, ProfileUpdateRequestAck, out );
    RETURN_AUTH_ERROR( in, profile_destruction_request, ProfileDestructionRequestAck, out );
    return false;
}

}  // namespace protocol

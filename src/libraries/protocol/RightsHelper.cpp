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
    if ( in##.message().has_##name##() )                                        \
    {                                                                           \
        out##.mutable_message()->mutable_##name##_ack()                         \
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

}  // namespace protocol

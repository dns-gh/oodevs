// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_Effect_DetectionRange.h"
#include "MIL_AgentServer.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"

// -----------------------------------------------------------------------------
// Name: MIL_Effect_DetectionRange constructor
// Created: JSR 2013-04-24
// -----------------------------------------------------------------------------
MIL_Effect_DetectionRange::MIL_Effect_DetectionRange( T_IdVector& perceiversIds, int resourceId, T_IdVector& fireEffectsIds )
{
    for( auto it = perceiversIds.begin(); it != perceiversIds.end(); ++it )
        msg_().add_perceivers()->set_id( *it );
    msg_().mutable_ammunition()->set_id( resourceId );
    for( auto it = fireEffectsIds.begin(); it != fireEffectsIds.end(); ++it )
        msg_().add_fire_effects()->set_id( *it );
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_DetectionRange destructor
// Created: JSR 2013-04-24
// -----------------------------------------------------------------------------
MIL_Effect_DetectionRange::~MIL_Effect_DetectionRange()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Effect_DetectionRange::Execute
// Created: JSR 2013-04-24
// -----------------------------------------------------------------------------
bool MIL_Effect_DetectionRange::Execute()
{
    msg_.Send( NET_Publisher_ABC::Publisher() );
    delete this;
    return false;
}

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "SinkRoleExtender.h"
#include "Entities/Agents/MIL_AgentPion.h"

SinkRoleExtender::SinkRoleExtender( RoleExtender_ABC* chain, boost::function< MIL_AgentPion&(MIL_AgentPion&) > configure )
    : chain_( chain )
    , configure_( configure )
{
    // NOTHING
}

void SinkRoleExtender::RegisterRoles( MIL_AgentPion& pion )
{
    if( chain_ )
        chain_->RegisterRoles( pion );
    configure_( pion );
}

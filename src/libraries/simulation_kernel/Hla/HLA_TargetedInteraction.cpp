// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2004-11-29 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Hla/HLA_TargetedInteraction.cpp $
// $Author: Age $
// $Modtime: 29/11/04 16:48 $
// $Revision: 1 $
// $Workfile: HLA_TargetedInteraction.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "HLA_TargetedInteraction.h"
#include "Entities/Agents/Roles/HLA/HLA_RoleInterface.h"
#include "MIL_AgentServer.h"
#include "HLA_Federate.h"

using namespace hla;

// -----------------------------------------------------------------------------
// Name: HLA_TargetedInteraction constructor
// Created: AGE 2004-11-29
// -----------------------------------------------------------------------------
HLA_TargetedInteraction::HLA_TargetedInteraction()
    : target_( uint(-1) )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: HLA_TargetedInteraction constructor
// Created: AGE 2004-11-29
// -----------------------------------------------------------------------------
HLA_TargetedInteraction::HLA_TargetedInteraction( const HLA_RoleInterface& hlaRole )
    : target_( hlaRole.GetId() )
{
    //NOTHING
}
 
// -----------------------------------------------------------------------------
// Name: HLA_TargetedInteraction destructor
// Created: AGE 2004-11-29
// -----------------------------------------------------------------------------
HLA_TargetedInteraction::~HLA_TargetedInteraction()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: HLA_TargetedInteraction::GetTarget
// Created: AGE 2004-11-29
// -----------------------------------------------------------------------------
MIL_Agent_ABC* HLA_TargetedInteraction::GetTarget() const
{
    assert( MIL_AgentServer::GetWorkspace().GetHLAFederate() );
    return MIL_AgentServer::GetWorkspace().GetHLAFederate()->GetLocalAgent( target_ );
}

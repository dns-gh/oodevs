// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "PHY_ActionIllumination.h"

#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Units/Dotations/PHY_AmmoDotationClass.h"
#include "Entities/Agents/Actions/Firing/PHY_FireResults_Pion.h"
#include "Decision/DEC_Tools.h"

using namespace firing;
// -----------------------------------------------------------------------------
// Name: PHY_ActionIllumination constructor
// Created: MGD 2010-02-12
// -----------------------------------------------------------------------------
PHY_ActionIllumination::PHY_ActionIllumination( MIL_AgentPion& pion, boost::shared_ptr< DEC_Knowledge_Agent > pEnemy )
    : PHY_DecisionCallbackAction_ABC( pion )
    , role_                       ( pion.GetRole< PHY_RoleAction_DirectFiring >() )
    , pEnemy_                     ( pEnemy )
{
    Callback( role_.GetInitialReturnCode() );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionIllumination destructor
// Created: MGD 2010-02-12
// -----------------------------------------------------------------------------
PHY_ActionIllumination::~PHY_ActionIllumination()
{
    Callback( role_.GetFinalReturnCode() );
}


// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_ActionIllumination::Execute
// Created: MGD 2010-02-12
// -----------------------------------------------------------------------------
void PHY_ActionIllumination::Execute()
{
    int nResult = role_.IlluminatePion( pEnemy_ );
    Callback( nResult );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionIllumination::ExecuteSuspended
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
void PHY_ActionIllumination::ExecuteSuspended()
{
    role_.FirePionSuspended( pEnemy_ );
}

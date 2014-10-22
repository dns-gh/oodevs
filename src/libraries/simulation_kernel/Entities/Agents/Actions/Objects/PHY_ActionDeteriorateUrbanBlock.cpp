// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_ActionDeteriorateUrbanBlock.h"
#include "PHY_RoleAction_Objects.h"
#include "Decision/DEC_Decision_ABC.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Urban/MIL_UrbanObject.h"

// -----------------------------------------------------------------------------
// Name: PHY_ActionDeteriorateUrbanBlock constructor
// Created: JSR 2012-04-19
// -----------------------------------------------------------------------------
PHY_ActionDeteriorateUrbanBlock::PHY_ActionDeteriorateUrbanBlock( MIL_AgentPion& pion, MIL_UrbanObject_ABC* pUrbanBlock, double percentage )
    : PHY_DecisionCallbackAction_ABC( pion )
    , role_       ( pion.GetRole< PHY_RoleAction_Objects >() )
    , pUrbanBlock_( pUrbanBlock )
    , percentage_ ( std::max( 1., percentage ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionDeteriorateUrbanBlock destructor
// Created: JSR 2012-04-19
// -----------------------------------------------------------------------------
PHY_ActionDeteriorateUrbanBlock::~PHY_ActionDeteriorateUrbanBlock()
{
        // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionDeteriorateUrbanBlock::Execute
// Created: JSR 2012-04-19
// -----------------------------------------------------------------------------
void PHY_ActionDeteriorateUrbanBlock::Execute()
{
    int nReturn = role_.DeteriorateUrbanBlock( pUrbanBlock_, percentage_ );
    Callback( nReturn );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionDeteriorateUrbanBlock::ExecuteSuspended
// Created: JSR 2012-04-19
// -----------------------------------------------------------------------------
void PHY_ActionDeteriorateUrbanBlock::ExecuteSuspended()
{
    role_.DeteriorateUrbanBlockSuspended();
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionDeteriorateUrbanBlock::StopAction
// Created: JSR 2012-04-19
// -----------------------------------------------------------------------------
void PHY_ActionDeteriorateUrbanBlock::StopAction()
{
    Callback( role_.GetFinalReturnCode() );
}

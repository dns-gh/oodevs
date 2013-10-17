// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_ActionResumeWorkUrbanBlock.h"
#include "PHY_RoleAction_Objects.h"
#include "Decision/DEC_Decision_ABC.h"
#include "Urban/MIL_UrbanObject_ABC.h"
#include "Entities/Agents/MIL_AgentPion.h"

// -----------------------------------------------------------------------------
// Name: PHY_ActionResumeWorkUrbanBlock constructor
// Created: LGY 2011-03-23
// -----------------------------------------------------------------------------
PHY_ActionResumeWorkUrbanBlock::PHY_ActionResumeWorkUrbanBlock( MIL_AgentPion& pion, MIL_UrbanObject_ABC* pUrbanBlock )
    : PHY_DecisionCallbackAction_ABC( pion )
    , role_         ( pion.GetRole< PHY_RoleAction_Objects >() )
    , pUrbanBlock_  ( pUrbanBlock )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionResumeWorkUrbanBlock destructor
// Created: LGY 2011-03-23
// -----------------------------------------------------------------------------
PHY_ActionResumeWorkUrbanBlock::~PHY_ActionResumeWorkUrbanBlock()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionResumeWorkUrbanBlock::Execute
// Created: LGY 2011-03-23
// -----------------------------------------------------------------------------
void PHY_ActionResumeWorkUrbanBlock::Execute()
{
    int nReturn = role_.ResumeWork( pUrbanBlock_ );
    Callback( nReturn );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionResumeWorkUrbanBlock::ExecuteSuspended
// Created: LGY 2011-03-23
// -----------------------------------------------------------------------------
void PHY_ActionResumeWorkUrbanBlock::ExecuteSuspended()
{
    role_.ResumeWorkSuspended();
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionResumeWorkUrbanBlock::StopAction
// Created: LGY 2011-03-23
// -----------------------------------------------------------------------------
void PHY_ActionResumeWorkUrbanBlock::StopAction()
{
    Callback( role_.GetFinalReturnCode() );
}

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_ActionUnloadCrowd.h"
#include "PHY_RoleAction_CrowdTransport.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Knowledge/DEC_Knowledge_Population.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_KnowledgeGroup.h"

using namespace crowdtransport;

// -----------------------------------------------------------------------------
// Name: PHY_ActionUnloadCrowd constructor
// Created: JSR 2011-08-09
// -----------------------------------------------------------------------------
PHY_ActionUnloadCrowd::PHY_ActionUnloadCrowd( MIL_AgentPion& pion, int knowledgeId, MT_Vector2D* position )
    : PHY_DecisionCallbackAction_ABC( pion )
    , role_    ( pion.GetRole< PHY_RoleAction_CrowdTransport >() )
    , position_( position )
{
    if( !position )
        throw std::runtime_error( __FUNCTION__ " Invalid position" );
    DEC_Knowledge_Population* pKnowledge = pion.GetKnowledgeGroup().GetKnowledge().GetKnowledgePopulationFromID( knowledgeId );
    if( !pKnowledge )
        throw std::runtime_error( __FUNCTION__ " Unknown crowd knowledge" );
    crowd_ = &pKnowledge->GetPopulationKnown();
    Callback( role_.GetInitialReturnCode() );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionUnloadCrowd destructor
// Created: JSR 2011-08-09
// -----------------------------------------------------------------------------
PHY_ActionUnloadCrowd::~PHY_ActionUnloadCrowd()
{
    Callback( role_.GetFinalReturnCode() );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionUnloadCrowd::Execute
// Created: JSR 2011-08-09
// -----------------------------------------------------------------------------
void PHY_ActionUnloadCrowd::Execute()
{
    int nResult = role_.UnloadCrowd( *crowd_, *position_ );
    Callback( nResult );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionUnloadCrowd::ExecuteSuspended
// Created: JSR 2011-08-09
// -----------------------------------------------------------------------------
void PHY_ActionUnloadCrowd::ExecuteSuspended()
{
    role_.UnloadSuspended();
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionUnloadCrowd::StopAction
// Created: JSR 2011-08-09
// -----------------------------------------------------------------------------
void PHY_ActionUnloadCrowd::StopAction()
{
    Callback( role_.GetFinalReturnCode() );
}

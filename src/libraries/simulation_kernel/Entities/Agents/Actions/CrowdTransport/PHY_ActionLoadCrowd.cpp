// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_ActionLoadCrowd.h"
#include "PHY_RoleAction_CrowdTransport.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Knowledge/DEC_Knowledge_Population.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_KnowledgeGroup.h"

using namespace crowdtransport;

// -----------------------------------------------------------------------------
// Name: PHY_ActionLoadCrowd constructor
// Created: JSR 2011-08-09
// -----------------------------------------------------------------------------
PHY_ActionLoadCrowd::PHY_ActionLoadCrowd( MIL_AgentPion& pion, int knowledgeId, unsigned int concentrationId )
    : PHY_DecisionCallbackAction_ABC( pion )
    , role_           ( pion.GetRole< PHY_RoleAction_CrowdTransport >() )
    , concentrationId_( concentrationId )
{
    DEC_Knowledge_Population* pKnowledge = pion.GetKnowledgeGroup().GetKnowledge().GetKnowledgePopulationFromID( knowledgeId );
    if( !pKnowledge )
        throw std::runtime_error( __FUNCTION__ " Unknown crowd knowledge" );
    crowd_ = &pKnowledge->GetPopulationKnown();
    //MIL_PopulationConcentration* concentration = pKnowledge->GetPopulationKnown().GetConcentration()
    Callback( role_.GetInitialReturnCode() );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionLoadCrowd destructor
// Created: JSR 2011-08-09
// -----------------------------------------------------------------------------
PHY_ActionLoadCrowd::~PHY_ActionLoadCrowd()
{
    Callback( role_.GetFinalReturnCode() );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionLoadCrowd::Execute
// Created: JSR 2011-08-09
// -----------------------------------------------------------------------------
void PHY_ActionLoadCrowd::Execute()
{
    int nResult = role_.LoadCrowd( *crowd_, concentrationId_ );
    Callback( nResult );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionLoadCrowd::ExecuteSuspended
// Created: JSR 2011-08-09
// -----------------------------------------------------------------------------
void PHY_ActionLoadCrowd::ExecuteSuspended()
{
    role_.LoadSuspended();
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionLoadCrowd::StopAction
// Created: JSR 2011-08-09
// -----------------------------------------------------------------------------
void PHY_ActionLoadCrowd::StopAction()
{
    Callback( role_.GetFinalReturnCode() );
}

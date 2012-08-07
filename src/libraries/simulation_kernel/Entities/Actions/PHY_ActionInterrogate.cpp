// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_ActionInterrogate.h"

#include "MIL_AgentServer.h"
#include "PopulationFactory_ABC.h"
#include "Entities/MIL_Army_ABC.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Populations/MIL_Population.h"
#include "Entities/Orders/MIL_Report.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_Knowledge_Population.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_AgentPion.h"
#include "Tools/MIL_AffinitiesMap.h"

// -----------------------------------------------------------------------------
// Name: PHY_ActionInterrogate Agent constructor
// Created: ABR 2011-03-25
// -----------------------------------------------------------------------------
PHY_ActionInterrogate::PHY_ActionInterrogate( MIL_AgentPion& caller, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge )
    : PHY_DecisionCallbackAction_ABC( caller )
    , rBaseTime_   ( MIL_AgentServer::GetWorkspace().GetRealTime() )
    , rTimeToWait_ ( 0 )
    , pKnowledge_  ( pKnowledge )
    , caller_      ( caller )
{
    unsigned int callerTeamID = caller.GetArmy().GetID();

    float affinity = pKnowledge->GetAgentKnown().GetAffinity( callerTeamID );
    ComputeTimeToWait( affinity );
    E_ReturnCode result = eRunning;
    if( affinity < 0 || ( pKnowledge_->IsAnEnemy( caller.GetArmy() ) == eTristate_True && caller_.GetKnowledge().GetRapForLocalValue() < 2.25 ) )
        result = eFailed;
    else
        MIL_Report::PostEvent( pKnowledge->GetAgentKnown(), MIL_Report::eReport_Questionning );
    Callback( static_cast< int >( result ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionInterrogate Crowd constructor
// Created: ABR 2011-03-25
// -----------------------------------------------------------------------------
PHY_ActionInterrogate::PHY_ActionInterrogate( MIL_AgentPion& caller, int knowledgeCrowdId )
    : PHY_DecisionCallbackAction_ABC( caller )
    , rBaseTime_   ( MIL_AgentServer::GetWorkspace().GetRealTime() )
    , rTimeToWait_ ( 0 )
    , caller_      ( caller )
{
    unsigned int callerTeamID = caller.GetArmy().GetID();
    DEC_Knowledge_Population* pKnowledge = caller.GetKnowledgeGroup().GetKnowledge().GetKnowledgePopulationFromID( knowledgeCrowdId );
    if( !pKnowledge )
        throw std::runtime_error( __FUNCTION__ " Unknown crowd knowledge" );
    MIL_Population& crowd = pKnowledge->GetPopulationKnown();
    ComputeTimeToWait( crowd.GetAffinity( callerTeamID ) );
    Callback( static_cast< int >( eRunning ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionInterrogate destructor
// Created: ABR 2011-03-25
// -----------------------------------------------------------------------------
PHY_ActionInterrogate::~PHY_ActionInterrogate()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionInterrogate::ComputeTimeToWait
// Created: ABR 2011-03-28
// -----------------------------------------------------------------------------
void PHY_ActionInterrogate::ComputeTimeToWait( float affinity )
{
    double diff = ( affinity > 0. )
        ? MIL_AffinitiesMap::interrogateDelayForNeutralAffinity_ - MIL_AffinitiesMap::interrogateDelayForMaximumAffinity_
        : MIL_AffinitiesMap::interrogateDelayForMinimumAffinity_ - MIL_AffinitiesMap::interrogateDelayForNeutralAffinity_;
    rTimeToWait_ = static_cast< unsigned int >( MIL_AffinitiesMap::interrogateDelayForNeutralAffinity_ - ( affinity * diff ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionInterrogate::Execute
// Created: ABR 2011-03-25
// -----------------------------------------------------------------------------
void PHY_ActionInterrogate::Execute()
{
    if( pKnowledge_ && ( pKnowledge_->IsAnEnemy( caller_.GetArmy() ) == eTristate_True && caller_.GetKnowledge().GetRapForLocalValue() < 2.25 ) )
         Callback( static_cast< int >( eFailed ) );
    else
    {
        if( MIL_AgentServer::GetWorkspace().GetRealTime() - rBaseTime_ < rTimeToWait_ )
             Callback( static_cast< int >( eRunning ) );
        else
             Callback( static_cast< int >( eFinished ) );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionInterrogate::ExecuteSuspended
// Created: ABR 2011-03-25
// -----------------------------------------------------------------------------
void PHY_ActionInterrogate::ExecuteSuspended()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionInterrogate::StopAction
// Created: ABR 2011-03-25
// -----------------------------------------------------------------------------
void PHY_ActionInterrogate::StopAction()
{
    // NOTHING
}

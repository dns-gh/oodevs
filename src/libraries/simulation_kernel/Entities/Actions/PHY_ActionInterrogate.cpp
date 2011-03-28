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
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Tools/MIL_AffinitiesMap.h"

// -----------------------------------------------------------------------------
// Name: PHY_ActionInterrogate Agent constructor
// Created: ABR 2011-03-25
// -----------------------------------------------------------------------------
PHY_ActionInterrogate::PHY_ActionInterrogate( MIL_AgentPion& caller, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge )
    : PHY_DecisionCallbackAction_ABC( caller )
    , rBaseTime_   ( MIL_AgentServer::GetWorkspace().GetSimTime() )
    , rTimeToWait_ ( 0 )
{
    unsigned int callerTeamID = caller.GetArmy().GetID();

    ComputeTimeToWait( pKnowledge->GetAgentKnown().GetAffinity( callerTeamID ) );
    Callback( static_cast< int >( eRunning ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionInterrogate Crowd constructor
// Created: ABR 2011-03-25
// -----------------------------------------------------------------------------
PHY_ActionInterrogate::PHY_ActionInterrogate( MIL_AgentPion& caller, int knowledgeCrowdId )
    : PHY_DecisionCallbackAction_ABC( caller )
    , rBaseTime_   ( MIL_AgentServer::GetWorkspace().GetSimTime() )
    , rTimeToWait_ ( 0 )
{
    unsigned int callerTeamID = caller.GetArmy().GetID();
    MIL_Population* crowd = MIL_AgentServer::GetWorkspace().GetEntityManager().FindPopulation( knowledgeCrowdId );

    if( !crowd )
        throw std::runtime_error( __FUNCTION__ " Unknown crowd" );
    ComputeTimeToWait( crowd->GetAffinity( callerTeamID ) );
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
    if( MIL_AgentServer::GetWorkspace().GetSimTime() - rBaseTime_ < rTimeToWait_ )
        Callback( static_cast< int >( eRunning ) );
    else
        Callback( static_cast< int >( eFinished ) );
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

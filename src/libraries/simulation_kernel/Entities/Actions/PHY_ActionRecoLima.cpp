// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_ActionRecoLima.h"
#include "MIL_AgentServer.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/MIL_Army_ABC.h"
#include "Entities/Orders/MIL_LimaFunction.h"
#include "Entities/Orders/MIL_LimaOrder.h"
#include "Entities/Orders/MIL_Report.h"
#include "Knowledge/DEC_BlackBoard_CanContainKnowledgeAgent.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include <boost/bind.hpp>


// -----------------------------------------------------------------------------
// Name: PHY_ActionRecoLima Agent constructor
// Created: MMC 2013-07-08
// -----------------------------------------------------------------------------
PHY_ActionRecoLima::PHY_ActionRecoLima( MIL_AgentPion& caller, const MIL_LimaFunction& limaType )
    : simTime_  ( MIL_AgentServer::GetWorkspace().GetSimTime() )
    , caller_   ( caller )
    , limaType_ ( limaType )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionRecoLima destructor
// Created: MMC 2013-07-08
// -----------------------------------------------------------------------------
PHY_ActionRecoLima::~PHY_ActionRecoLima()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionRecoLima::Execute
// Created: MMC 2013-07-08
// -----------------------------------------------------------------------------
void PHY_ActionRecoLima::Execute()
{
    if( !ComputeLimas() )
        return;
    previousAgentsPos_.swap( agentsPos_ );
    agentsPos_.clear();
    std::function< void( DEC_Knowledge_Agent& ) > agentFunctor = boost::bind( &PHY_ActionRecoLima::CheckAgentKnowledgesNearLimas, this, _1 );
    const MIL_Automate& automat = caller_.GetAutomate();
    if( !automat.IsEngaged() || ( automat.IsEngaged() && IsFirstInAutomatValidWithMission() ) )
    {
        caller_.GetKnowledgeGroup()->ApplyOnKnowledgesAgent( agentFunctor );
        CheckAgentsNearLimas( caller_.GetKnowledgeGroup()->GetAutomates() );
        auto groups = caller_.GetKnowledgeGroup()->GetKnowledgeGroups();
        for( auto it = groups.begin(); it != groups.end(); ++it )
            CheckAgentsNearLimas( (*it)->GetAutomates() );
        for( auto it = lineCrossedAgents_.begin(); it != lineCrossedAgents_.end(); ++it )
        {
            if( automat.IsEngaged() )
            {
                if( automat.GetPionPC() && !automat.GetPionPC()->IsJammed() )
                    MIL_Report::PostEvent( automat, report::eRC_CrossedLima, limaType_, (*it)->GetName() );
            }
            else
                MIL_Report::PostEvent( caller_, report::eRC_CrossedLima, limaType_, (*it)->GetName() );
        }
    }
    lineCrossedAgents_.clear();
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionRecoLima::ComputeLimas
// Created: MMC 2013-10-11
// -----------------------------------------------------------------------------
bool PHY_ActionRecoLima::ComputeLimas()
{
    limas_.clear();
    if( caller_.GetOrderManager().GetCurrentMissionType() && caller_.GetKnowledgeGroup().get()
        && !caller_.GetOrderManager().GetLimas().empty() )
    {
        const T_LimaVector& limas = caller_.GetOrderManager().GetLimas();
        for( auto it = limas.begin(); it != limas.end(); ++it )
            if( it->HasFunction( limaType_ )  )
                limas_.push_back( &( *it ) );
    }
    return !limas_.empty();
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionRecoLima::CheckAgentKnowledgesNearLimas
// Created: MMC 2013-07-08
// -----------------------------------------------------------------------------
void PHY_ActionRecoLima::CheckAgentKnowledgesNearLimas( const DEC_Knowledge_Agent& agent )
{
    if( agent.GetMaxPerceptionLevel() == PHY_PerceptionLevel::identified_ )
        CheckAgentsNearLimas( agent.GetAgentKnown() );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionRecoLima::CheckAgentsNearLimas
// Created: MMC 2013-07-08
// -----------------------------------------------------------------------------
void PHY_ActionRecoLima::CheckAgentsNearLimas( const std::vector< MIL_Automate* >& automats )
{
    for( auto automat = automats.begin(); automat != automats.end(); ++automat )
        for( auto pion = (*automat)->GetPions().begin(); pion != (*automat)->GetPions().end(); ++pion )
            CheckAgentsNearLimas( **pion );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionRecoLima::CheckAgentsNearLimas
// Created: MMC 2013-07-08
// -----------------------------------------------------------------------------
void PHY_ActionRecoLima::CheckAgentsNearLimas( const MIL_Agent_ABC& agent )
{
    if( limas_.empty() || caller_.GetArmy().IsAFriend( agent.GetArmy() ) != eTristate_True )
        return;
    for( auto it = limas_.begin(); it != limas_.end(); ++it )
    {
        auto curPosition = agent.GetRole< PHY_RoleInterface_Location >().GetPosition();
        const MIL_LimaOrder& lima = **it;
        if( caller_.GetAutomate().GetID() == agent.GetAutomate().GetID() )
            continue;
        if( lima.GetLocalisation()->IsOnBorder( curPosition, 1000.0 ) )
        {
            agentsPos_[ &agent ] = curPosition;
            auto itPrevious = previousAgentsPos_.find( &agent );
            if( itPrevious != previousAgentsPos_.end() )
            {
                MT_Vector2D intersection;
                if( lima.Intersect2D( MT_Line( itPrevious->second, curPosition ), intersection ) )
                    lineCrossedAgents_.insert( &agent );
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionRecoLima::IsFirstInAutomatValidWithMission
// Created: MMC 2013-07-08
// -----------------------------------------------------------------------------
bool PHY_ActionRecoLima::IsFirstInAutomatValidWithMission()
{
    auto pions = caller_.GetAutomate().GetPions();
    for( auto it = pions.begin(); it != pions.end(); ++it )
        if( caller_.GetOrderManager().GetMissionName() == (*it)->GetOrderManager().GetMissionName() )
            return !caller_.IsJammed() && caller_.GetID() == (*it)->GetID();
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionRecoLima::ExecuteSuspended
// Created: MMC 2013-07-08
// -----------------------------------------------------------------------------
void PHY_ActionRecoLima::ExecuteSuspended()
{
    // NOTHING
}

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_ActionRecoLIA.h"
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
// Name: PHY_ActionRecoLIA Agent constructor
// Created: MMC 2013-07-08
// -----------------------------------------------------------------------------
PHY_ActionRecoLIA::PHY_ActionRecoLIA( MIL_AgentPion& caller )
    : simTime_  ( MIL_AgentServer::GetWorkspace().GetSimTime() )
    , caller_   ( caller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionRecoLIA destructor
// Created: MMC 2013-07-08
// -----------------------------------------------------------------------------
PHY_ActionRecoLIA::~PHY_ActionRecoLIA()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionRecoLIA::Execute
// Created: MMC 2013-07-08
// -----------------------------------------------------------------------------
void PHY_ActionRecoLIA::Execute()
{
    if( !caller_.GetKnowledgeGroup().get() )
        return;
    previousAgentsPos_.swap( agentsPos_ );
    agentsPos_.clear();
    boost::function< void( DEC_Knowledge_Agent& ) > agentFunctor = boost::bind( &PHY_ActionRecoLIA::CheckAgentKnowledgesNearLimasLIA, this, _1 );
    if( caller_.GetAutomate().IsEngaged() ? IsFirstInAutomatWithMission() : true )
    {
        caller_.GetKnowledgeGroup()->ApplyOnKnowledgesAgent( agentFunctor );
        CheckAgentsNearLimasLIA( caller_.GetKnowledgeGroup()->GetAutomates() );
        auto groups = caller_.GetKnowledgeGroup()->GetKnowledgeGroups();
        for( auto it = groups.begin(); it != groups.end(); ++it )
            CheckAgentsNearLimasLIA( (*it)->GetAutomates() );
        for( auto it = lineCrossedAgents_.begin(); it != lineCrossedAgents_.end(); ++it )
        {
            auto longName = (*it)->GetLongName();
            if( caller_.GetAutomate().IsEngaged() )
                MIL_Report::PostEvent( caller_.GetAutomate(), MIL_Report::eRC_UnitCrossedLimaLIA, longName.empty()? (*it)->GetName() : longName );
            else
                MIL_Report::PostEvent( caller_, MIL_Report::eRC_UnitCrossedLimaLIA, longName.empty()? (*it)->GetName() : longName );
        }
    }
    lineCrossedAgents_.clear();
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionRecoLIA::CheckAgentKnowledgesNearLimasLIA
// Created: MMC 2013-07-08
// -----------------------------------------------------------------------------
void PHY_ActionRecoLIA::CheckAgentKnowledgesNearLimasLIA( const DEC_Knowledge_Agent& agent )
{
    if( agent.GetMaxPerceptionLevel() != PHY_PerceptionLevel::identified_ )
        return;
    if( !agent.GetArmy() )
        return;
    MIL_AgentPion* pAgentPion = dynamic_cast< MIL_AgentPion* >( &agent.GetAgentKnown() );
    if( pAgentPion )
        CheckAgentsNearLimasLIA( *pAgentPion );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionRecoLIA::CheckAgentsNearLimasLIA
// Created: MMC 2013-07-08
// -----------------------------------------------------------------------------
void PHY_ActionRecoLIA::CheckAgentsNearLimasLIA( const std::vector< MIL_Automate* >& automats )
{
    for( auto automat = automats.begin(); automat != automats.end(); ++automat )
        for( auto pion = (*automat)->GetPions().begin(); pion != (*automat)->GetPions().end(); ++pion )
            CheckAgentsNearLimasLIA( **pion );
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionRecoLIA::CheckAgentsNearLimasLIA
// Created: MMC 2013-07-08
// -----------------------------------------------------------------------------
void PHY_ActionRecoLIA::CheckAgentsNearLimasLIA( const MIL_AgentPion& agent )
{
    if( caller_.GetArmy().IsAFriend( agent.GetArmy() ) != eTristate_True )
        return;
    const T_LimaVector& limas = caller_.GetOrderManager().GetLimas();
    for( auto it = limas.begin(); it != limas.end(); ++it )
    {
        auto curPosition = agent.GetRole< PHY_RoleInterface_Location >().GetPosition();
        const MIL_LimaOrder& lima = *it;
        if( ( caller_.GetAutomate().GetID() != agent.GetAutomate().GetID() ) && lima.HasFunction( MIL_LimaFunction::LIA_ ) 
            && lima.GetLocalisation()->IsOnBorder( curPosition, 1000.0 ) )
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
// Name: PHY_ActionRecoLIA::ExecuteSuspended
// Created: MMC 2013-07-08
// -----------------------------------------------------------------------------
bool PHY_ActionRecoLIA::IsFirstInAutomatWithMission()
{
    auto pions = caller_.GetAutomate().GetPions();
    for( auto it = pions.begin(); it != pions.end(); ++it )
        if( caller_.GetOrderManager().GetMissionName() == (*it)->GetOrderManager().GetMissionName() )
            return caller_.GetID() == (*it)->GetID();
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionRecoLIA::ExecuteSuspended
// Created: MMC 2013-07-08
// -----------------------------------------------------------------------------
void PHY_ActionRecoLIA::ExecuteSuspended()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionRecoLIA::StopAction
// Created: MMC 2013-07-08
// -----------------------------------------------------------------------------
void PHY_ActionRecoLIA::StopAction()
{
    // NOTHING
}

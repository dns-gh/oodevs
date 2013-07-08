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
    previousAgents_.swap( agents_ );
    agents_.clear();
    boost::function< void( DEC_Knowledge_Agent& ) > agentFunctor = boost::bind( &PHY_ActionRecoLIA::CheckAgentsNearLimasLIA, this, _1 );
    caller_.GetKnowledgeGroup()->ApplyOnKnowledgesAgent( agentFunctor );
    for( auto it = lineCrossedAgents_.begin(); it != lineCrossedAgents_.end(); ++it )
        MIL_Report::PostEvent( (*it)->GetAgentKnown(), MIL_Report::eRC_FriendUnitIdentified );
    lineCrossedAgents_.clear();
}

// -----------------------------------------------------------------------------
// Name: PHY_ActionRecoLIA::Execute
// Created: MMC 2013-07-08
// -----------------------------------------------------------------------------
void PHY_ActionRecoLIA::CheckAgentsNearLimasLIA( DEC_Knowledge_Agent& agent )
{
    const T_LimaVector& limas = caller_.GetOrderManager().GetLimas();
    for( auto it = limas.begin(); it != limas.end(); ++it )
    {
        const MIL_LimaOrder& lima = *it;
        if( lima.HasFunction( MIL_LimaFunction::LIA_ ) && lima.GetLocalisation()->IsOnBorder( agent.GetPosition(), 500.0 ) )
        {
            agents_[ &agent ] = agent.GetPosition();
            auto itPrevious = previousAgents_.find( &agent );
            if( ( itPrevious != previousAgents_.end() ) && itPrevious->first->IsValid() )
            {
                MT_Vector2D intersection;
                MT_Line line = MT_Line( itPrevious->first->GetPosition(), agent.GetPosition() );
                if( lima.Intersect2D( line, intersection ) )
                    lineCrossedAgents_.insert( &agent );
            }
        }
    }
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

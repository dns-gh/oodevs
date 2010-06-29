// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_KnowledgeUrbanFunctions.h"

#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Agents/Roles/Posture/PHY_RoleInterface_Posture.h"
#include "Entities/Agents/Roles/Urban/PHY_RoleInterface_UrbanLocation.h"
#include "Entities/Objects/UrbanObjectWrapper.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_Knowledge_Urban.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "urban/MaterialCompositionType.h"
#include "urban/TerrainObject_ABC.h"
#include "urban/Architecture.h"

#include "MT_Tools/MT_LinearInterpolation.h"


// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeUrbanFunctions::GetCurrentPerceptionLevel
// Created: SLG 2010-02-01
// -----------------------------------------------------------------------------
int DEC_KnowledgeUrbanFunctions::GetCurrentPerceptionLevel( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Urban > pKnowledge )
{
    if( pKnowledge.get() && pKnowledge->IsValid() )
        return (int)pKnowledge->GetCurrentPerceptionLevel( callerAgent ).GetID();
    else
        return 0;
}


// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeUrbanFunctions::GetCurrentBarycenter
// Created: MGD 2010-02-19
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_KnowledgeUrbanFunctions::GetCurrentBarycenter( boost::shared_ptr< DEC_Knowledge_Urban > pKnowledge )
{
    boost::shared_ptr< MT_Vector2D > pos;
    if( pKnowledge.get() && pKnowledge->IsValid() )
    {
        geometry::Point2f temp = pKnowledge->GetBarycenter();
        pos.reset( new MT_Vector2D( temp.X(), temp.Y() ) );
    }
    return pos;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeUrbanFunctions::GetPathfindCost
// Created: MGD 2010-03-18
// -----------------------------------------------------------------------------
float DEC_KnowledgeUrbanFunctions::GetPathfindCost( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Urban > pKnowledge )
{
     if( pKnowledge.get() && pKnowledge->IsValid() )
        return pKnowledge->GetPathfindCost( (float)callerAgent.GetRole< PHY_RoleInterface_Composantes >().GetMajorComponentWeight() );
     else
        return -1;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeUrbanFunctions::GetRapForLocal
// Created: MGD 2010-05-05
// -----------------------------------------------------------------------------
float DEC_KnowledgeUrbanFunctions::GetRapForLocal( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Urban > pKnowledge )
{
    //@TODO MGD Add a rapFor computer, common with DEC_Knowledge_RapForLocal
    T_KnowledgeAgentVector dangerousEnemies_;

    double rTotalFightScoreEnemy  = 0;
    double rTotalFightScoreFriend = 0;

    const T_KnowledgeAgentVector& enemies = callerAgent.GetKnowledgeGroup().GetKnowledge().GetEnemies();
    for( CIT_KnowledgeAgentVector it = enemies.begin(); it != enemies.end(); it++ )
    {
        if( (*it)->IsInUrbanBlock( pKnowledge->GetTerrainObjectKnown() ) )
        {
            rTotalFightScoreEnemy += (float)(*it)->GetDangerosity( callerAgent );
            dangerousEnemies_.push_back( *it );
        }
    }

    const T_KnowledgeAgentVector& allies = callerAgent.GetKnowledgeGroup().GetKnowledge().GetFriends();
    for( CIT_KnowledgeAgentVector it = allies.begin(); it != allies.end(); it++ )
    {
        if( (*it)->IsInUrbanBlock( pKnowledge->GetTerrainObjectKnown() ) )
        {
            double rTotalDangerosity = 0.;
            for( CIT_ConstKnowledgeAgentVector itAgentEnemy = dangerousEnemies_.begin(); itAgentEnemy != dangerousEnemies_.end(); ++itAgentEnemy )
                rTotalDangerosity += ( (*it)->GetDangerosity( **itAgentEnemy ) * (*it)->GetOperationalState() );
            rTotalFightScoreFriend += ( rTotalDangerosity / dangerousEnemies_.size() );
        }
    }

    double rRapForValue = 1.;
    if( rTotalFightScoreEnemy != 0. )
        rRapForValue = rTotalFightScoreFriend / rTotalFightScoreEnemy;

    // Add bonus if the pion is posted in this urbanbloc
    const UrbanObjectWrapper* urbanBlock = callerAgent.GetRole< PHY_RoleInterface_UrbanLocation >().GetCurrentUrbanBlock();
    if( urbanBlock && &pKnowledge->GetTerrainObjectKnown() == &urbanBlock->GetObject() && callerAgent.GetRole< PHY_RoleInterface_Posture >().IsInstalled() )
        rRapForValue *= 1.20;

    if( rRapForValue < 0.2 )
        rRapForValue = 0.2;
    if( rRapForValue > 5.0 )
        rRapForValue = 5.0;

    return (float)rRapForValue;
}

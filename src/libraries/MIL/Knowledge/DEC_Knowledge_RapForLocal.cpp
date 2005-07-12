// *****************************************************************************
//
// $Created: NLD 2004-04-07 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Knowledge/DEC_Knowledge_RapForLocal.cpp $
// $Author: Nld $
// $Modtime: 25/11/04 17:07 $
// $Revision: 2 $
// $Workfile: DEC_Knowledge_RapForLocal.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"
#include "DEC_Knowledge_RapForLocal.h"

#include "DEC_Knowledge_Agent.h"
#include "Entities/Agents/MIL_AgentPion.h"

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_RapForLocal constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_RapForLocal::DEC_Knowledge_RapForLocal()
    : DEC_Knowledge_RapFor_ABC()
{

}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_RapForLocal destructor
// Created: NLD 2004-04-07
// -----------------------------------------------------------------------------
DEC_Knowledge_RapForLocal::~DEC_Knowledge_RapForLocal()
{
    
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_RapForLocal::Update
// Created: NLD 2004-04-07
// -----------------------------------------------------------------------------
void DEC_Knowledge_RapForLocal::Update( const MIL_AgentPion& pion, const T_KnowledgeAgentVector& enemies, const T_KnowledgeAgentVector& friends )
{
    T_KnowledgeAgentVector agentLocalEnemies;

    MT_Float rTotalFightScoreEnemy  = 0;
    MT_Float rTotalFightScoreFriend = 0;

    // 1 - Compute the enemy fight score, and get the dangerous enemies
    for( CIT_KnowledgeAgentVector itEnemy = enemies.begin(); itEnemy != enemies.end(); ++itEnemy )
    {
        DEC_Knowledge_Agent& knowledgeEnemy = **itEnemy;
        MT_Float rDangerosity = knowledgeEnemy.GetDangerosity( pion ) * knowledgeEnemy.GetEtatOps();
        if( rDangerosity != 0. )
        {
            rTotalFightScoreEnemy += rDangerosity;
            agentLocalEnemies.push_back( &knowledgeEnemy );
        }
    }

    // 2 - Compute the friend fight scores against the agent local enemies
    if( !agentLocalEnemies.empty() )
    {
        for( CIT_KnowledgeAgentVector itFriend = friends.begin(); itFriend != friends.end(); ++itFriend )
        {
            DEC_Knowledge_Agent& knowledgeFriend = **itFriend;
            MT_Float rTotalDangerosity = 0.;
            for( CIT_KnowledgeAgentVector itAgentEnemy = agentLocalEnemies.begin(); itAgentEnemy != agentLocalEnemies.end(); ++itAgentEnemy )
                rTotalDangerosity += ( knowledgeFriend.GetDangerosity( **itAgentEnemy ) * knowledgeFriend.GetEtatOps() );
            rTotalFightScoreFriend += ( rTotalDangerosity / agentLocalEnemies.size() );
        }
    }
    
    ApplyValue( rTotalFightScoreFriend, rTotalFightScoreEnemy, pion.GetType().GetRapForIncreasePerTimeStepValue() );
}

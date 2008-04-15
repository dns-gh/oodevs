// *****************************************************************************
//
// $Created: NLD 2004-04-07 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Knowledge/DEC_Knowledge_RapForGlobal.cpp $
// $Author: Nld $
// $Modtime: 25/11/04 17:14 $
// $Revision: 2 $
// $Workfile: DEC_Knowledge_RapForGlobal.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Knowledge_RapForGlobal.h"

#include "DEC_Knowledge_Agent.h"
#include "DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "MIL_KnowledgeGroup.h"
#include "MIL_AgentServer.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Automates/MIL_AutomateType.h"
#include "Entities/Agents/MIL_AgentPion.h"

BOOST_CLASS_EXPORT_GUID( DEC_Knowledge_RapForGlobal, "DEC_Knowledge_RapForGlobal" )

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_RapForGlobal constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_RapForGlobal::DEC_Knowledge_RapForGlobal( const MIL_Automate& automate )
    : DEC_Knowledge_RapFor_ABC()
    , pAutomate_              ( &automate )
{

}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_RapForGlobal constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_RapForGlobal::DEC_Knowledge_RapForGlobal()
    : DEC_Knowledge_RapFor_ABC()
    , pAutomate_              ( 0 )
{

}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_RapForGlobal destructor
// Created: NLD 2004-04-07
// -----------------------------------------------------------------------------
DEC_Knowledge_RapForGlobal::~DEC_Knowledge_RapForGlobal()
{
    
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: template< typename Archive > void DEC_Knowledge_RapForGlobal::serialize
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
template< typename Archive > 
void DEC_Knowledge_RapForGlobal::serialize( Archive& archive, const uint )
{
    archive & boost::serialization::base_object< DEC_Knowledge_RapFor_ABC >( *this )
            & const_cast< MIL_Automate*& >( pAutomate_ );
}


// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_RapForGlobal::Update
// Created: NLD 2004-04-07
// -----------------------------------------------------------------------------
void DEC_Knowledge_RapForGlobal::Update()
{
    if( nLastCacheUpdateTick_ >= MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() )
        return;
    nLastCacheUpdateTick_ = MIL_AgentServer::GetWorkspace().GetCurrentTimeStep();

    assert( pAutomate_ );
    const T_KnowledgeAgentVector& enemies = pAutomate_->GetKnowledgeGroup().GetKnowledge().GetEnemies();
    const T_KnowledgeAgentVector& friends = pAutomate_->GetKnowledgeGroup().GetKnowledge().GetFriends();

    T_KnowledgeAgentVector automateLocalEnemies;

    MT_Float rTotalFightScoreEnemy  = 0;
    MT_Float rTotalFightScoreFriend = 0;

    // 1 - Compute the enemy fight score, and get the dangerous enemies for the automate and its subordinates
    for( CIT_KnowledgeAgentVector itEnemy = enemies.begin(); itEnemy != enemies.end(); ++itEnemy )
    {
        const MIL_Automate::T_PionVector& pions = pAutomate_->GetPions();

        DEC_Knowledge_Agent& knowledgeEnemy = **itEnemy;
        MT_Float rTotalDangerosity = 0.;
        uint nNbrPionAlive = 0;
        for( MIL_Automate::CIT_PionVector itPion = pions.begin(); itPion != pions.end(); ++itPion )
        {
            if( (*itPion)->IsDead() )
                continue;
            rTotalDangerosity += ( knowledgeEnemy.GetDangerosity( **itPion ) * knowledgeEnemy.GetOperationalState() );
            ++nNbrPionAlive;
        }

        if( rTotalDangerosity != 0. )
        {
            rTotalFightScoreEnemy += ( rTotalDangerosity / ( nNbrPionAlive + 1 ) );
            automateLocalEnemies.push_back( &knowledgeEnemy );
        }
    }

    // 2 - Compute the friend fight scores against the agent local enemies
    if( !automateLocalEnemies.empty() )
    {
        for( CIT_KnowledgeAgentVector itFriend = friends.begin(); itFriend != friends.end(); ++itFriend )
        {
            DEC_Knowledge_Agent& knowledgeFriend = **itFriend;
            MT_Float rTotalDangerosity = 0.;
            for( CIT_KnowledgeAgentVector itAgentEnemy = automateLocalEnemies.begin(); itAgentEnemy != automateLocalEnemies.end(); ++itAgentEnemy )
                rTotalDangerosity += ( knowledgeFriend.GetDangerosity( **itAgentEnemy ) * knowledgeFriend.GetOperationalState() );
            rTotalFightScoreFriend += ( rTotalDangerosity / automateLocalEnemies.size() );
        }
    }
    
    ApplyValue( rTotalFightScoreFriend, rTotalFightScoreEnemy, pAutomate_->GetType().GetRapForIncreasePerTimeStepValue() );
}

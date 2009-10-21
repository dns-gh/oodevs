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

#include "simulation_kernel_pch.h"
#include "DEC_Knowledge_RapForLocal.h"

#include "DEC_Knowledge_Agent.h"
#include "DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "MIL_KnowledgeGroup.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "MIL_AgentServer.h"

BOOST_CLASS_EXPORT_GUID( DEC_Knowledge_RapForLocal, "DEC_Knowledge_RapForLocal" )

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_RapForLocal constructor
// Created: NLD 2006-04-21
// -----------------------------------------------------------------------------
DEC_Knowledge_RapForLocal::DEC_Knowledge_RapForLocal( const MIL_AgentPion& pion )
    : DEC_Knowledge_RapFor_ABC()
    , pPion_                  ( &pion )
    , dangerousEnemies_       ()
{

}
    
// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_RapForLocal constructor
// Created: NLD 2006-04-21
// -----------------------------------------------------------------------------
DEC_Knowledge_RapForLocal::DEC_Knowledge_RapForLocal()
    : DEC_Knowledge_RapFor_ABC()
    , pPion_                  ( 0 )
    , dangerousEnemies_       ()
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
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: template< typename Archive > void DEC_Knowledge_RapForLocal::serialize
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
template< typename Archive > 
void DEC_Knowledge_RapForLocal::serialize( Archive& archive, const uint )
{
    archive & boost::serialization::base_object< DEC_Knowledge_RapFor_ABC >( *this )
            & const_cast< MIL_AgentPion*& >( pPion_ );

    assert( pPion_ );
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_RapForLocal::Update
// Created: NLD 2004-04-07
// -----------------------------------------------------------------------------
void DEC_Knowledge_RapForLocal::Update()
{
    if( nLastCacheUpdateTick_ >= MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() )
        return;
    nLastCacheUpdateTick_ = MIL_AgentServer::GetWorkspace().GetCurrentTimeStep();

    assert( pPion_ );
    const T_KnowledgeAgentVector& enemies = pPion_->GetKnowledgeGroup().GetKnowledge().GetEnemies();
    const T_KnowledgeAgentVector& friends = pPion_->GetKnowledgeGroup().GetKnowledge().GetFriends();

    dangerousEnemies_.clear();
    
    MT_Float rTotalFightScoreEnemy  = 0;
    MT_Float rTotalFightScoreFriend = 0;

    // 1 - Compute the enemy fight score, and get the dangerous enemies
    for( CIT_KnowledgeAgentVector itEnemy = enemies.begin(); itEnemy != enemies.end(); ++itEnemy )
    {
        boost::shared_ptr< DEC_Knowledge_Agent > knowledgeEnemy = *itEnemy;
        MT_Float rDangerosity = knowledgeEnemy->GetDangerosity( *pPion_ ) * knowledgeEnemy->GetOperationalState();
        if( rDangerosity != 0. )
        {
            rTotalFightScoreEnemy += rDangerosity;
            dangerousEnemies_.push_back( knowledgeEnemy );
        }
    }

    // 2 - Compute the friend fight scores against the agent local enemies
    if( !dangerousEnemies_.empty() )
    {
        for( CIT_KnowledgeAgentVector itFriend = friends.begin(); itFriend != friends.end(); ++itFriend )
        {
            DEC_Knowledge_Agent& knowledgeFriend = **itFriend;
            MT_Float rTotalDangerosity = 0.;
            for( CIT_ConstKnowledgeAgentVector itAgentEnemy = dangerousEnemies_.begin(); itAgentEnemy != dangerousEnemies_.end(); ++itAgentEnemy )
                rTotalDangerosity += ( knowledgeFriend.GetDangerosity( **itAgentEnemy ) * knowledgeFriend.GetOperationalState() );
            rTotalFightScoreFriend += ( rTotalDangerosity / dangerousEnemies_.size() );
        }
    }
    
    ApplyValue( rTotalFightScoreFriend, rTotalFightScoreEnemy, pPion_->GetType().GetRapForIncreasePerTimeStepValue() );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_RapForLocal::GetDangerousEnemies
// Created: NLD 2006-01-24
// -----------------------------------------------------------------------------
const T_ConstKnowledgeAgentVector& DEC_Knowledge_RapForLocal::GetDangerousEnemies()
{
    Update();
    return dangerousEnemies_;
}


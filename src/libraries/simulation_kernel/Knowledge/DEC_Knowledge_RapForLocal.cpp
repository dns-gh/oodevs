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

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_Knowledge_RapForLocal )

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_RapForLocal constructor
// Created: NLD 2006-04-21
// -----------------------------------------------------------------------------
DEC_Knowledge_RapForLocal::DEC_Knowledge_RapForLocal( const MIL_Agent_ABC& pion )
    : DEC_Knowledge_RapFor_ABC()
    , pPion_( &pion )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_RapForLocal constructor
// Created: NLD 2006-04-21
// -----------------------------------------------------------------------------
DEC_Knowledge_RapForLocal::DEC_Knowledge_RapForLocal()
    : DEC_Knowledge_RapFor_ABC()
    , pPion_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_RapForLocal destructor
// Created: NLD 2004-04-07
// -----------------------------------------------------------------------------
DEC_Knowledge_RapForLocal::~DEC_Knowledge_RapForLocal()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_RapForLocal::serialize
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
template< typename Archive >
void DEC_Knowledge_RapForLocal::serialize( Archive& archive, const unsigned int )
{
    archive & boost::serialization::base_object< DEC_Knowledge_RapFor_ABC >( *this )
            & const_cast< MIL_Agent_ABC*& >( pPion_ );
    assert( pPion_ );
}

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
    double rTotalFightScoreEnemy  = 0;
    double rTotalFightScoreFriend = 0;

    // 1 - Compute the enemy fight score, and get the dangerous enemies
    for( CIT_KnowledgeAgentVector itEnemy = enemies.begin(); itEnemy != enemies.end(); ++itEnemy )
    {
        boost::shared_ptr< DEC_Knowledge_Agent > knowledgeEnemy = *itEnemy;
        double rDangerosity = knowledgeEnemy->GetDangerosity( *pPion_ ) * knowledgeEnemy->GetOperationalState();
        if( rDangerosity != 0. )
        {
            rTotalFightScoreEnemy += rDangerosity;
            dangerousEnemies_.push_back( knowledgeEnemy );
        }
    }

    // 2 - Compute the friend fight scores against the agent local enemies
    int enemiesSize = dangerousEnemies_.size();
    if( enemiesSize )
    {
        for( CIT_KnowledgeAgentVector itFriend = friends.begin(); itFriend != friends.end(); ++itFriend )
        {
            DEC_Knowledge_Agent& knowledgeFriend = **itFriend;
            double rTotalDangerosity = 0.;
            double operationalState = knowledgeFriend.GetOperationalState();
            for( CIT_ConstKnowledgeAgentVector itAgentEnemy = dangerousEnemies_.begin(); itAgentEnemy != dangerousEnemies_.end(); ++itAgentEnemy )
                rTotalDangerosity += ( knowledgeFriend.GetDangerosity( **itAgentEnemy ) * operationalState );
            rTotalFightScoreFriend += ( rTotalDangerosity / enemiesSize );
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

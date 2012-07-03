// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "Knowledge_RapForLocal.h"
#include "WeaponType.h"
#include <wrapper/Hook.h>

using namespace sword;
using namespace sword::fire;

DECLARE_HOOK( EvaluateDangerosity, double, ( const SWORD_Model* agent, const SWORD_Model* target ) ) // agent->GetDangerosity( *pPion_, false ) * agent->GetOperationalState()
DECLARE_HOOK( EvaluateDangerosity2, double, ( const SWORD_Model* agent, const SWORD_Model* target ) ) // agent->GetDangerosity( *target, true ) * agent->GetOperationalState()

// -----------------------------------------------------------------------------
// Name: Knowledge_RapForLocal::Update
// Created: NLD 2004-04-07
// -----------------------------------------------------------------------------
void Knowledge_RapForLocal::Update( const wrapper::View& model, const wrapper::View& entity )
{
    if( nLastCacheUpdateTick_ >= model[ "tick" ] )
        return;
    nLastCacheUpdateTick_ = model[ "tick" ];
    const unsigned int id = entity[ "knowledges" ];
    const wrapper::View& knowledges = model[ "knowledges" ][ id ];
    dangerousEnemies_.clear();
    double rTotalFightScoreEnemy  = 0;
    double rTotalFightScoreFriend = 0;
    // 1 - Compute the enemy fight score, and get the dangerous enemies
    const wrapper::View& enemies = model[ "enemies" ][ id ];
    for( std::size_t i = 0; i < enemies.GetSize(); ++i )
    {
        const SWORD_Model* knowledgeEnemy = knowledges[ static_cast< unsigned int >( enemies.GetElement( i ) ) ];
        double rDangerosity = GET_HOOK( EvaluateDangerosity )( knowledgeEnemy, entity );
        if( rDangerosity != 0. )
        {
            rTotalFightScoreEnemy += rDangerosity;
            dangerousEnemies_.push_back( knowledgeEnemy );
        }
    }
    // 2 - Compute the friend fight scores against the agent local enemies
    int enemiesSize = (int) dangerousEnemies_.size();
    if( enemiesSize )
    {
        const wrapper::View& friends = model[ "friends" ][ id ];
        for( std::size_t i = 0; i < friends.GetSize(); ++i )
        {
            const SWORD_Model* knowledgeFriend = knowledges[ static_cast< unsigned int >( friends.GetElement( i ) ) ];
            double rTotalDangerosity = 0.;
            for( CIT_KnowledgeAgents itAgentEnemy = dangerousEnemies_.begin(); itAgentEnemy != dangerousEnemies_.end(); ++itAgentEnemy )
                rTotalDangerosity += GET_HOOK( EvaluateDangerosity2 )( knowledgeFriend, *itAgentEnemy );
            rTotalFightScoreFriend += ( rTotalDangerosity / enemiesSize );
        }
    }
    ApplyValue( rTotalFightScoreFriend, rTotalFightScoreEnemy, entity[ "fire/force-ratio/feedback-time" ] );
}

// -----------------------------------------------------------------------------
// Name: Knowledge_RapForLocal::GetDangerousEnemies
// Created: NLD 2006-01-24
// -----------------------------------------------------------------------------
const Knowledge_RapForLocal::T_KnowledgeAgents& Knowledge_RapForLocal::GetDangerousEnemies( const wrapper::View& model, const wrapper::View& entity )
{
    Update( model, entity );
    return dangerousEnemies_;
}

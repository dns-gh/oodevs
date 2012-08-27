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

DEFINE_HOOK( ComputeForceRatio, double, ( const SWORD_Model* model, const SWORD_Model* entity,
    bool(*filter)( const SWORD_Model* knowledge, void* userData ), void* userData ) )
{
    std::vector< const SWORD_Model* > agents;
    std::pair< double, double > scores = Knowledge_RapForLocal().GetRapForLocal( model, entity, agents, filter, userData );
    double rRapForValue = 1;
    if( scores.second != 0 )
        rRapForValue = scores.first / scores.second;
    return rRapForValue;
}

// -----------------------------------------------------------------------------
// Name: Knowledge_RapForLocal::GetRapForLocal
// Created: MCO 2012-08-27
// -----------------------------------------------------------------------------
std::pair< double, double > Knowledge_RapForLocal::GetRapForLocal( const wrapper::View& model, const wrapper::View& entity, T_KnowledgeAgents& dangerousEnemies, bool(*filter)( const SWORD_Model* knowledge, void* userData ), void* userData ) const
{
    const unsigned int id = entity[ "knowledges" ];
    const wrapper::View& knowledges = model[ "knowledges" ][ id ];
    double rTotalFightScoreEnemy  = 0;
    double rTotalFightScoreFriend = 0;
    // 1 - Compute the enemy fight score, and get the dangerous enemies
    const wrapper::View& enemies = model[ "enemies" ][ id ];
    for( std::size_t i = 0; i < enemies.GetSize(); ++i )
    {
        const SWORD_Model* knowledgeEnemy = knowledges[ static_cast< unsigned int >( enemies.GetElement( i ) ) ];
        if( ! filter( knowledgeEnemy, userData ) )
            continue;
        double rDangerosity = GET_HOOK( EvaluateDangerosity )( knowledgeEnemy, entity );
        if( rDangerosity != 0 )
        {
            rTotalFightScoreEnemy += rDangerosity;
            dangerousEnemies.push_back( knowledgeEnemy );
        }
    }
    // 2 - Compute the friend fight scores against the agent local enemies
    int enemiesSize = (int) dangerousEnemies.size();
    if( enemiesSize )
    {
        const wrapper::View& friends = model[ "friends" ][ id ];
        for( std::size_t i = 0; i < friends.GetSize(); ++i )
        {
            const SWORD_Model* knowledgeFriend = knowledges[ static_cast< unsigned int >( friends.GetElement( i ) ) ];
            if( ! filter( knowledgeFriend, userData ) )
                continue;
            double rTotalDangerosity = 0.;
            for( CIT_KnowledgeAgents itAgentEnemy = dangerousEnemies.begin(); itAgentEnemy != dangerousEnemies.end(); ++itAgentEnemy )
                rTotalDangerosity += GET_HOOK( EvaluateDangerosity2 )( knowledgeFriend, *itAgentEnemy );
            rTotalFightScoreFriend += ( rTotalDangerosity / enemiesSize );
        }
    }
    return std::make_pair( rTotalFightScoreFriend, rTotalFightScoreEnemy );
}

namespace
{
    bool FilterNothing( const SWORD_Model* /*knowledge*/, void* /*userData*/ )
    {
        return true;
    }
}

// -----------------------------------------------------------------------------
// Name: Knowledge_RapForLocal::Update
// Created: NLD 2004-04-07
// -----------------------------------------------------------------------------
void Knowledge_RapForLocal::Update( const wrapper::View& model, const wrapper::View& entity )
{
    if( nLastCacheUpdateTick_ >= model[ "tick" ] )
        return;
    nLastCacheUpdateTick_ = model[ "tick" ];
    dangerousEnemies_.clear();
    std::pair< double, double > result = GetRapForLocal( model, entity, dangerousEnemies_, FilterNothing, 0 );
    ApplyValue( result.first, result.second, entity[ "fire/force-ratio/feedback-time" ] );
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

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
#include "tools/Codec.h" // $$$$ MCO 2012-05-15: refactor dependency
#include <wrapper/Hook.h>
#include <module_api/Log.h>
#include <xeumeuleu/xml.h>

using namespace sword;
using namespace sword::fire;

DECLARE_HOOK( EvaluateDangerosity, double, ( const SWORD_Model* agent, const SWORD_Model* target ) )
DECLARE_CACHED_HOOK( EvaluateDangerosity2, 2, double, ( const SWORD_Model* agent, const SWORD_Model* target ) )

double Knowledge_RapForLocal::rRapForIncreasePerTimeStepDefaultValue_ = 0;

namespace
{
    DEFINE_HOOK( InitializeDecisional, 2, void, ( const char* xml, double tickDuration ) )
    {
        // $$$$ MCO : TODO : maybe we need to store configuration data in a model somehow ?
        xml::xistringstream xis( xml );
        xis >> xml::start( "decisional" );
        Knowledge_RapForLocal::Initialize( xis, tickDuration );
        if( GET_PREVIOUS_HOOK( InitializeDecisional ) )
            GET_PREVIOUS_HOOK( InitializeDecisional )( xml, tickDuration );
    }

    const double rRapForBoundMin = 0.2;
    const double rRapForBoundMax = 5;

    double ComputeRapForIncreasePerTimeStepValue( double rBaseTimeValue )
    {
        if( rBaseTimeValue <= 0. )
            return rRapForBoundMax - rRapForBoundMin;
        return ( rRapForBoundMax - rRapForBoundMin ) / rBaseTimeValue;
    }

    double GetRapForLocal( const wrapper::View& model, const wrapper::View& entity, Knowledge_RapForLocal::T_KnowledgeAgents& dangerousEnemies, bool(*filter)( const SWORD_Model* knowledge, void* userData ), void* userData, double defaultValue )
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
        if( dangerousEnemies.empty() )
            return defaultValue;
        const wrapper::View& friends = model[ "friends" ][ id ];
        for( std::size_t i = 0; i < friends.GetSize(); ++i )
        {
            const SWORD_Model* knowledgeFriend = knowledges[ static_cast< unsigned int >( friends.GetElement( i ) ) ];
            if( ! filter( knowledgeFriend, userData ) )
                continue;
            for( Knowledge_RapForLocal::CIT_KnowledgeAgents it = dangerousEnemies.begin(); it != dangerousEnemies.end(); ++it )
                rTotalFightScoreFriend += GET_HOOK( EvaluateDangerosity2 )( knowledgeFriend, *it );
        }
        return ( rTotalFightScoreFriend / dangerousEnemies.size() ) / rTotalFightScoreEnemy;
    }

    DEFINE_HOOK( ComputeForceRatio, 4, double, ( const SWORD_Model* model, const SWORD_Model* entity,
                                              bool(*filter)( const SWORD_Model* knowledge, void* userData ), void* userData ) )
    {
        std::vector< const SWORD_Model* > agents;
        return GetRapForLocal( model, entity, agents, filter, userData, 1 );
    }
}

// -----------------------------------------------------------------------------
// Name: Knowledge_RapForLocal constructor
// Created: SLI 2012-10-17
// -----------------------------------------------------------------------------
Knowledge_RapForLocal::Knowledge_RapForLocal()
    : rRapForValue_        ( rRapForBoundMax )
    , nLastCacheUpdateTick_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Knowledge_RapForLocal::Initialize
// Created: NLD 2004-11-25
// -----------------------------------------------------------------------------
void Knowledge_RapForLocal::Initialize( xml::xisubstream xis, double tickDuration )
{
    xis >> xml::start( "force-ratio" );
    double rTmp = 0;
    tools::ReadTimeAttribute( xis, "default-feedback-time", rTmp );
    rRapForIncreasePerTimeStepDefaultValue_ = ComputeRapForIncreasePerTimeStepValue( rTmp / tickDuration );
}

namespace
{
    bool FilterNothing( const SWORD_Model* /*knowledge*/, void* /*userData*/ )
    {
        return true;
    }
}

// -----------------------------------------------------------------------------
// Name: Knowledge_RapForLocal::ApplyValue
// Created: NLD 2004-04-08
// -----------------------------------------------------------------------------
void Knowledge_RapForLocal::ApplyValue( double rNewRapForValue, double rFeedbackTime )
{
    // Bound the value between 0.2 and 5.0
    static double rRapForMidValue_ = ( rRapForBoundMax - rRapForBoundMin ) / 2;
    if( rNewRapForValue < 1. )
        rNewRapForValue = ( (rRapForMidValue_ - rRapForBoundMin) * rNewRapForValue ) + rRapForBoundMin;
    else
        rNewRapForValue = std::min( rNewRapForValue + (rRapForMidValue_ - 1), rRapForBoundMax );
    if( rNewRapForValue <= rRapForValue_ )
        rRapForValue_ = rNewRapForValue;
    else
        rRapForValue_ += std::min( rNewRapForValue - rRapForValue_, ComputeRapForIncreasePerTimeStepValue( rFeedbackTime ) );
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
    const double ratio = GetRapForLocal( model, entity, dangerousEnemies_, FilterNothing, 0, std::numeric_limits< double >::max() );
    ApplyValue( ratio, entity[ "fire/force-ratio/feedback-time" ] );
}

// -----------------------------------------------------------------------------
// Name: Knowledge_RapForLocal::GetDangerousEnemies
// Created: NLD 2006-01-24
// -----------------------------------------------------------------------------
const Knowledge_RapForLocal::T_KnowledgeAgents& Knowledge_RapForLocal::GetDangerousEnemies() const
{
    return dangerousEnemies_;
}

// -----------------------------------------------------------------------------
// Name: Knowledge_RapForLocal::GetValue
// Created: NLD 2004-04-07
// -----------------------------------------------------------------------------
double Knowledge_RapForLocal::GetValue() const
{
    return rRapForValue_;
}

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
#include <boost/container/flat_map.hpp>

using namespace sword;
using namespace sword::fire;

DECLARE_HOOK( EvaluateDangerosity, double, ( const SWORD_Model* agent, const SWORD_Model* target ) )
DECLARE_HOOK( EvaluateDangerosity2, double, ( const SWORD_Model* agent, const SWORD_Model* target ) )

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

    typedef std::vector< double > T_DangerosityCache;
    std::map< std::size_t, T_DangerosityCache > caches;

    void FillCache( T_DangerosityCache& cache, std::size_t id, const wrapper::View& knowledges, const wrapper::View& model )
    {
        const wrapper::View& friends = model[ "friends" ][ id ];
        const wrapper::View& enemies = model[ "enemies" ][ id ];
        const std::size_t fs = friends.GetSize();
        const std::size_t es = enemies.GetSize();
        cache.reserve( fs * es );
        for( std::size_t i = 0; i < fs; ++i )
        {
            const SWORD_Model* f = knowledges[ static_cast< std::size_t >( friends.GetElement( i ) ) ];
            for( std::size_t j = 0; j < es; ++j )
            {
                const SWORD_Model* e = knowledges[ static_cast< std::size_t >( enemies.GetElement( j ) ) ];
                cache.push_back( GET_HOOK( EvaluateDangerosity2 )( f, e ) );
            }
        }
    }
    const T_DangerosityCache& GetCache( const wrapper::View& model, const wrapper::View& entity )
    {
        const std::size_t k = entity[ "knowledges" ];
        T_DangerosityCache& cache = caches[ k ];
        if( cache.empty() )
            FillCache( cache, k, model[ "knowledges" ][ k ][ "agents" ], model );
        return cache;
    }

    DEFINE_HOOK( Tick, 0, void, () )
    {
        caches.clear();
        if( GET_PREVIOUS_HOOK( Tick ) )
            GET_PREVIOUS_HOOK( Tick )();
    }

    double GetRapForLocal( const wrapper::View& model, const wrapper::View& entity, Knowledge_RapForLocal::T_KnowledgeAgents& dangerousEnemies, bool(*filter)( const SWORD_Model* knowledge, void* userData ), void* userData, double defaultValue )
    {
        const T_DangerosityCache& cache = GetCache( model, entity );
        const std::size_t id = entity[ "knowledges" ];
        const wrapper::View& knowledges = model[ "knowledges" ][ id ][ "agents" ];
        double rTotalFightScoreEnemy  = 0;
        double rTotalFightScoreFriend = 0;
        // 1 - Compute the enemy fight score, and get the dangerous enemies
        const wrapper::View& enemies = model[ "enemies" ][ id ];
        std::vector< std::size_t > ids;
        for( std::size_t i = 0; i < enemies.GetSize(); ++i )
        {
            const SWORD_Model* knowledgeEnemy = knowledges[ static_cast< std::size_t >( enemies.GetElement( i ) ) ];
            if( ! filter( knowledgeEnemy, userData ) )
                continue;
            double rDangerosity = GET_HOOK( EvaluateDangerosity )( knowledgeEnemy, entity );
            if( rDangerosity != 0 )
            {
                rTotalFightScoreEnemy += rDangerosity;
                dangerousEnemies.push_back( knowledgeEnemy );
                ids.push_back( i );
            }
        }
        // 2 - Compute the friend fight scores against the agent local enemies
        if( dangerousEnemies.empty() )
            return defaultValue;
        const wrapper::View& friends = model[ "friends" ][ id ];
        for( std::size_t i = 0; i < friends.GetSize(); ++i )
        {
            const SWORD_Model* knowledgeFriend = knowledges[ static_cast< std::size_t >( friends.GetElement( i ) ) ];
            if( ! filter( knowledgeFriend, userData ) )
                continue;
            for( auto it = ids.begin(); it != ids.end(); ++it )
                rTotalFightScoreFriend += cache[ i * *it ];
        }
        return ( rTotalFightScoreFriend / dangerousEnemies.size() ) / rTotalFightScoreEnemy;
    }

    DEFINE_HOOK( ComputeForceRatio, 4, double, ( const SWORD_Model* model, const SWORD_Model* entity,
                                              bool(*filter)( const SWORD_Model* knowledge, void* userData ), void* userData ) )
    {
        std::vector< const SWORD_Model* > agents;
        return GetRapForLocal( model, entity, agents, filter, userData, 1 );
    }

    bool FilterNothing( const SWORD_Model* /*knowledge*/, void* /*userData*/ )
    {
        return true;
    }
}

// -----------------------------------------------------------------------------
// Name: Knowledge_RapForLocal constructor
// Created: SLI 2012-10-17
// -----------------------------------------------------------------------------
Knowledge_RapForLocal::Knowledge_RapForLocal( const wrapper::View& model, const wrapper::View& entity )
    : rRapForValue_( rRapForBoundMax )
{
    const double ratio = GetRapForLocal( model, entity, dangerousEnemies_, &FilterNothing, 0, std::numeric_limits< double >::max() );
    ApplyValue( ratio, entity[ "fire/force-ratio/feedback-time" ] );
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

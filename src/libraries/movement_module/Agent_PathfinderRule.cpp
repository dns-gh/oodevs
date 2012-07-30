// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Agent_PathfinderRule.h"
#include "Agent_Path.h"
#include "Agent_PathClass.h"
#include "PathPoint.h"
#include "wrapper/Hook.h"

using namespace sword::movement;

DECLARE_HOOK( GetAltitudeCost, double, ( const SWORD_Model* entity, boost::shared_ptr< sword::movement::Path_ABC > path, const MT_Vector2D& from, const MT_Vector2D& to, double rAltitudeCostPerMeter ) )
DECLARE_HOOK( GetFuseauxCost, double, ( const SWORD_Model* entity, boost::shared_ptr< sword::movement::Path_ABC > path, const MT_Vector2D& from, const MT_Vector2D& to,
              double rMaximumFuseauDistance, double rMaximumFuseauDistanceWithAutomata, // $$$$ MCO : all those configuration values should stay out of the movement module
              double rFuseauCostPerMeterOut, double rComfortFuseauDistance, double rFuseauCostPerMeterIn,
              double rMaximumAutomataFuseauDistance, double rAutomataFuseauCostPerMeterOut ) )
DECLARE_HOOK( GetEnemiesCost, double, ( const SWORD_Model* entity, boost::shared_ptr< sword::movement::Path_ABC > path, const MT_Vector2D& from, const MT_Vector2D& to, const TerrainData& nToTerrainType,
              const TerrainData& nLinkTerrainType, double rEnemyMaximumCost ) )
DECLARE_HOOK( GetObjectsCost, double, ( const SWORD_Model* entity, boost::shared_ptr< sword::movement::Path_ABC > path, const MT_Vector2D& from, const MT_Vector2D& to, const TerrainData& nToTerrainType, const TerrainData& nLinkTerrainType ) )
DECLARE_HOOK( GetPopulationsCost, double, ( const SWORD_Model* entity, boost::shared_ptr< sword::movement::Path_ABC > path, const MT_Vector2D& from, const MT_Vector2D& to,
              const TerrainData& nToTerrainType, const TerrainData& nLinkTerrainType, double rPopulationMaximumCost ) )
DECLARE_HOOK( GetUrbanBlockCost, double, ( const SWORD_Model* entity, boost::shared_ptr< sword::movement::Path_ABC > path, const MT_Vector2D& from, const MT_Vector2D& to ) )
DECLARE_HOOK( IsValidPosition, bool, ( const MT_Vector2D& point ) )

// -----------------------------------------------------------------------------
// Name: Agent_PathfinderRule constructor
// Created: AGE 2005-03-23
// -----------------------------------------------------------------------------
Agent_PathfinderRule::Agent_PathfinderRule( const sword::wrapper::View& entity, Agent_Path& path, const MT_Vector2D& from, const MT_Vector2D& to )
    : TerrainRule_ABC                ()
    , entity_                        ( entity )
    , path_                          ( path )
    , rMaxSpeed_                     ( float( path.GetUnitSpeeds().GetMaxSpeed() ) * 1.1f )
    , bShort_                        ( path.GetPathClass().IsShort() )
    , avoidedTerrain_                ( path.GetPathClass().GetAvoidedTerrain() )
    , rAvoidedTerrainCost_           ( avoidedTerrain_ == TerrainData() ? 0. : path.GetPathClass().GetAvoidedTerrainCost() )
    , preferedTerrain_               ( path.GetPathClass().GetPreferedTerrain() )
    , rPreferedTerrainCost_          ( preferedTerrain_ == TerrainData() ? 0. : path.GetPathClass().GetPreferedTerrainCost() )
    , rAltitudeCostPerMeter_         ( path.GetPathClass().GetAltitudePreference() )
    , rComfortFuseauDistance_        ( path.GetPathClass().GetComfortFuseauDistance() )
    , rFuseauCostPerMeterOut_        ( path.GetPathClass().GetFuseauCostPerMeterOut() )
    , rFuseauCostPerMeterIn_         ( path.GetPathClass().GetFuseauCostPerMeterIn () )
    , rAutomataFuseauCostPerMeterOut_( path.GetPathClass().GetAutomateFuseauCostPerMeterOut() )
    , dangerDirection_               ( MT_Vector2D( entity[ "danger/x" ], entity[ "danger/y" ] ) )
    , rDangerDirectionBaseCost_      ( path.GetPathClass().GetDangerDirectionBaseCost  () )
    , rDangerDirectionLinearCost_    ( path.GetPathClass().GetDangerDirectionLinearCost() )
    , dangerPoint_                   ( DotProduct( dangerDirection_, from ) > DotProduct( dangerDirection_, to ) ? from : to )
    , rEnemyMaximumCost_             ( path.GetPathClass().GetEnemyMaximumCost() )
    , rPopulationMaximumCost_        ( path.GetPathClass().GetPopulationMaximumCost() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Agent_PathfinderRule destructor
// Created: AGE 2005-03-08
// -----------------------------------------------------------------------------
Agent_PathfinderRule::~Agent_PathfinderRule()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Agent_PathfinderRule::GetTerrainCost
// Created: AGE 2005-03-25
// -----------------------------------------------------------------------------
double Agent_PathfinderRule::GetTerrainCost( const TerrainData& data ) const
{
    double rDynamicCost = 0;
    if( rAvoidedTerrainCost_ > 0 && avoidedTerrain_.ContainsOne( data ) )
        rDynamicCost += rAvoidedTerrainCost_;

    if( rPreferedTerrainCost_ > 0 && ! preferedTerrain_.ContainsOne( data ) )
        rDynamicCost += rPreferedTerrainCost_;
    return rDynamicCost;
}

// -----------------------------------------------------------------------------
// Name: Agent_PathfinderRule::GetDangerDirectionCost
// Created: NLD 2006-01-31
// -----------------------------------------------------------------------------
double Agent_PathfinderRule::GetDangerDirectionCost( const MT_Vector2D& to ) const
{
    // direction dangereuse
    if( !dangerDirection_.IsZero() )
    {
        const double dp = DotProduct( to - dangerPoint_, dangerDirection_ );
        if( dp > 0 ) // we are beyond our danger point
            return ( rDangerDirectionBaseCost_ + dp * rDangerDirectionLinearCost_ );
    }
    return 0.;
}

//#define DEBUG_IMPOSSIBLE_PATHFIND

#ifdef DEBUG_IMPOSSIBLE_PATHFIND

namespace
{
    double LogImpossible( const MT_Vector2D& from, const MT_Vector2D& to, const char* reason )
    {
        MT_LOG_INFO_MSG( "Impossible from " << from << " to " << to << " : " << reason );
        return -1.;
    }

    double LogImpossible( const MT_Vector2D& at, const char* reason )
    {
        MT_LOG_INFO_MSG( "Impossible at " << at << " : " << reason );
        return -1.;
    }
}

#   define IMPOSSIBLE_DESTINATION( reason ) LogImpossible( to, reason )
#   define IMPOSSIBLE_WAY( reason )         LogImpossible( from, to, reason )
#else // DEBUG_IMPOSSIBLE_PATHFIND
#   define IMPOSSIBLE_DESTINATION( reason ) -1.
#   define IMPOSSIBLE_WAY( reason )         -1.
#endif // DEBUG_IMPOSSIBLE_PATHFIND

namespace
{
    double GetPopulationAttitudeCost( unsigned int type, const void* userData )
    {
        return static_cast< const Agent_PathClass* >( userData )->GetPopulationAttitudeCost( type );
    }
}

// -----------------------------------------------------------------------------
// Name: Agent_PathfinderRule::GetCost
// Created: AGE 2005-03-08
// -----------------------------------------------------------------------------
double Agent_PathfinderRule::GetCost( const MT_Vector2D& from, const MT_Vector2D& to, const TerrainData& nToTerrainType, const TerrainData& nLinkTerrainType ) // $$$$ MCO 2012-05-23: executed in pathfind threads !
{
    if( ! GET_HOOK( IsValidPosition )( to ) )
        return IMPOSSIBLE_DESTINATION( "Out of world" );

    // speed
    double rSpeed = path_.GetUnitSpeeds().GetMaxSpeed( nLinkTerrainType );
    if( rSpeed <= 0. )
        return IMPOSSIBLE_WAY( "Speeds on terrain" );

    if( ! path_.GetUnitSpeeds().IsPassable( nToTerrainType ) )
        return IMPOSSIBLE_DESTINATION( "Terrain type" );

    const double rAltitudeCost = GET_HOOK( GetAltitudeCost )( entity_, path_.shared_from_this(), from, to, rAltitudeCostPerMeter_ );
    if( rAltitudeCost < 0 )
        return IMPOSSIBLE_WAY( "Slope" );

    // Cost computation taken various dynamic terrain items into account
    double rDynamicCost = 0.;

    // Altitude
    rDynamicCost += rAltitudeCost;

    // Fuseaux
    const double rFuseauxCost = GET_HOOK( GetFuseauxCost )( entity_, path_.shared_from_this(), from, to,
        path_.GetPathClass().GetMaximumFuseauDistance(), path_.GetPathClass().GetMaximumFuseauDistanceWithAutomata(), // $$$$ MCO : all those configuration values should stay out of the movement module
        rFuseauCostPerMeterOut_, rComfortFuseauDistance_, rFuseauCostPerMeterIn_,
        path_.GetPathClass().GetMaximumAutomataFuseauDistance(), rAutomataFuseauCostPerMeterOut_ );
    if( rFuseauxCost < 0. )
        return -1.;
    rDynamicCost += rFuseauxCost;

    // terrain type
    const double rTerrainCost = GetTerrainCost( nLinkTerrainType );
    rDynamicCost += rTerrainCost;

    // danger direction
    const double rDangerDirectionCost = GetDangerDirectionCost( to );
    rDynamicCost += rDangerDirectionCost;

    //urban blocks
    const double rUrbanBlockCost = path_.GetPathClass().IsFlying() ? 0 : GET_HOOK( GetUrbanBlockCost )( entity_, path_.shared_from_this(), from, to );

    if( rUrbanBlockCost < 0. )
        return IMPOSSIBLE_WAY( "Urban" );
    rDynamicCost += rUrbanBlockCost;

    // objects
    const double rObjectsCost =
        (! path_.GetPathClass().AvoidObjects() || path_.GetPathClass().IsFlying() )
        ? 0 : GET_HOOK( GetObjectsCost )( entity_, path_.shared_from_this(), from, to, nToTerrainType, nLinkTerrainType );
    if( rObjectsCost < 0 || rSpeed <= 0. )
        return IMPOSSIBLE_WAY( "Objects" );
    rDynamicCost += rObjectsCost;

    // enemies
    const double rEnemiesCost = GET_HOOK( GetEnemiesCost )( entity_, path_.shared_from_this(), from, to, nToTerrainType, nLinkTerrainType, rEnemyMaximumCost_ );
    if( rEnemiesCost < 0 )
        return IMPOSSIBLE_WAY( "Enemies" );
    rDynamicCost += rEnemiesCost;

    // populations
    const double rPopulationsCost = path_.GetPathClass().HandlePopulations()
        ? GET_HOOK( GetPopulationsCost )( entity_, path_.shared_from_this(), from, to, nToTerrainType, nLinkTerrainType, path_.GetPathClass().GetPopulationMaximumCost() )
        : 0;
    if( rPopulationsCost < 0 )
        return IMPOSSIBLE_WAY( "Populations" );
    rDynamicCost += rPopulationsCost;

    const double rDistance     = from.Distance( to );
    const double rBaseCost = bShort_ ? rDistance : ( rDistance / rSpeed );
    return rBaseCost * ( 1 + rDynamicCost );
}

// -----------------------------------------------------------------------------
// Name: Agent_PathfinderRule::EvaluateCost
// Created: AGE 2005-03-08
// -----------------------------------------------------------------------------
float Agent_PathfinderRule::EvaluateCost( const geometry::Point2f& from, const geometry::Point2f& to )
{
    const float rDistance = from.Distance( to );
    if( rMaxSpeed_ != 0 )
        return bShort_ ? rDistance : ( rDistance / rMaxSpeed_ );
    return FLT_MAX;
}

// -----------------------------------------------------------------------------
// Name: Agent_PathfinderRule::GetCost
// Created: AGE 2005-03-08
// -----------------------------------------------------------------------------
float Agent_PathfinderRule::GetCost( const geometry::Point2f& from, const geometry::Point2f& to, const TerrainData& terrainTo, const TerrainData& terrainBetween )
{
    return float( GetCost( MT_Vector2D( from.X(), from.Y() ),
                           MT_Vector2D( to.X(), to.Y() ),
                           terrainTo,
                           terrainBetween ) );
}

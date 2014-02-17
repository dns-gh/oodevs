// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Agent_PathfinderRule.h"
#include "DEC_Agent_Path.h"
#include "DEC_Agent_PathClass.h"
#include "DEC_Path_KnowledgeObject_ABC.h"
#include "DEC_Path_KnowledgePopulation.h"
#include "DEC_Path_KnowledgeAgent.h"
#include "MIL_AgentServer.h"
#include "SlopeSpeedModifier.h"
#include "simulation_terrain/TER_World.h"
#include "Tools/MIL_Tools.h"
#include "Urban/MIL_UrbanCache.h"
#include "Meteo/RawVisionData/PHY_RawVisionData.h"
#include "Meteo/RawVisionData/Elevation.h"
#include "Meteo/PHY_MeteoDataManager.h"

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathfinderRule::InitializeFuseauData
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
void DEC_Agent_PathfinderRule::InitializeFuseauData( const MT_Vector2D& from, const MT_Vector2D& to )
{
    assert( pFuseau_ == 0 );
    if( path_.GetFuseau().IsNull() )
        return;
    pFuseau_ = &path_.GetFuseau();
    rMaximumFuseauDistance_ = path_.GetPathClass().GetMaximumFuseauDistance();
    if( !pFuseau_->IsInside( from ) )
        rMaximumFuseauDistance_ += pFuseau_->Distance( from ) * 1.3; // $$$$ AGE 2005-06-08:
    if( !path_.GetAutomataFuseau().IsNull() ) // I have a higher fuseau
        rMaximumFuseauDistance_ = std::max( rMaximumFuseauDistance_, path_.GetPathClass().GetMaximumFuseauDistanceWithAutomata() );
    if( !pFuseau_->IsInside( to ) ) // our destination is outside
        rMaximumFuseauDistance_ = std::numeric_limits< double >::max();
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathfinderRule::InitializeAutomateFuseauData
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
void DEC_Agent_PathfinderRule::InitializeAutomateFuseauData( const MT_Vector2D& from )
{
    assert( pAutomateFuseau_ == 0 );
    if( path_.GetAutomataFuseau().IsNull() )
        return;
    pAutomateFuseau_ = &path_.GetAutomataFuseau();
    rMaximumAutomataFuseauDistance_ = path_.GetPathClass().GetMaximumAutomataFuseauDistance();
    if( !pAutomateFuseau_->IsInside( from ) )
        rMaximumAutomataFuseauDistance_ += pAutomateFuseau_->Distance( from ) * 1.3; // $$$$ AGE
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathfinderRule constructor
// Created: AGE 2005-03-23
// -----------------------------------------------------------------------------
DEC_Agent_PathfinderRule::DEC_Agent_PathfinderRule( const DEC_Agent_Path& path, const MT_Vector2D& from, const MT_Vector2D& to )
    : TerrainRule_ABC                ()
    , path_                          ( path )
    , world_                         ( TER_World::GetWorld() )
    , altitudeData_                  ( MIL_AgentServer::GetWorkspace().GetMeteoDataManager().GetRawVisionData() )
    , rMaxSpeed_                     ( float( path.GetUnitSpeeds().GetMaxSpeed() ) * 1.1f )
    , bShort_                        ( path.GetPathClass().IsShort() )
    , avoidedTerrain_                ( path.GetPathClass().GetAvoidedTerrain() )
    , rAvoidedTerrainCost_           ( avoidedTerrain_ == TerrainData() ? 0. : path.GetPathClass().GetAvoidedTerrainCost() )
    , preferedTerrain_               ( path.GetPathClass().GetPreferedTerrain() )
    , rPreferedTerrainCost_          ( preferedTerrain_ == TerrainData() ? 0. : path.GetPathClass().GetPreferedTerrainCost() )
    , rMinAltitude_                  ( MIL_AgentServer::GetWorkspace().GetMeteoDataManager().GetRawVisionData().GetMinAltitude() )
    , rMaxAltitude_                  ( MIL_AgentServer::GetWorkspace().GetMeteoDataManager().GetRawVisionData().GetMaxAltitude() )
    , rAltitudeCostPerMeter_         ( path.GetPathClass().GetAltitudePreference() )
    , rMaxSlope_                     ( path.GetUnitMaxSlope() )
    , rSlopeDeceleration_            ( path.GetUnitSlopeDeceleration() )
    , pFuseau_                       ( 0 )
    , rComfortFuseauDistance_        ( path.GetPathClass().GetComfortFuseauDistance() )
    , rFuseauCostPerMeterOut_        ( path.GetPathClass().GetFuseauCostPerMeterOut() )
    , rFuseauCostPerMeterIn_         ( path.GetPathClass().GetFuseauCostPerMeterIn () )
    , rMaximumFuseauDistance_        ( 0. ) // Initialized below
    , pAutomateFuseau_               ( 0 )
    , rAutomataFuseauCostPerMeterOut_( path.GetPathClass().GetAutomateFuseauCostPerMeterOut() )
    , rMaximumAutomataFuseauDistance_( 0. ) // Initialized below
    , dangerDirection_               ( path.GetDirDanger() )
    , rDangerDirectionBaseCost_      ( path.GetPathClass().GetDangerDirectionBaseCost  () )
    , rDangerDirectionLinearCost_    ( path.GetPathClass().GetDangerDirectionLinearCost() )
    , dangerPoint_                   ( DotProduct( dangerDirection_, from ) > DotProduct( dangerDirection_, to ) ? from : to )
    , rEnemyMaximumCost_             ( path.GetPathClass().GetEnemyMaximumCost() )
    , rPopulationMaximumCost_        ( path.GetPathClass().GetPopulationMaximumCost() )
{
    InitializeFuseauData( from, to );
    InitializeAutomateFuseauData( from );
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathfinderRule destructor
// Created: AGE 2005-03-08
// -----------------------------------------------------------------------------
DEC_Agent_PathfinderRule::~DEC_Agent_PathfinderRule()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathfinderRule::GetTerrainCost
// Created: AGE 2005-03-25
// -----------------------------------------------------------------------------
double DEC_Agent_PathfinderRule::GetTerrainCost( const TerrainData& data ) const
{
    double rDynamicCost = 0;
    if( rAvoidedTerrainCost_ > 0 && avoidedTerrain_.ContainsOne( data ) )
        rDynamicCost += rAvoidedTerrainCost_;

    if( rPreferedTerrainCost_ > 0 && ! preferedTerrain_.ContainsOne( data ) )
        rDynamicCost += rPreferedTerrainCost_;
    return rDynamicCost;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathfinderRule::GetDangerDirectionCost
// Created: NLD 2006-01-31
// -----------------------------------------------------------------------------
double DEC_Agent_PathfinderRule::GetDangerDirectionCost( const MT_Vector2D& to ) const
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

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathfinderRule::GetUrbanBlockCost
// Created: RPD 2009-11-20
// -----------------------------------------------------------------------------
double DEC_Agent_PathfinderRule::GetUrbanBlockCost( const MT_Vector2D& from, const MT_Vector2D& to ) const
{
    if( MIL_AgentServer::IsInitialized() && !path_.GetPathClass().IsFlying() )
        return MIL_AgentServer::GetWorkspace().GetUrbanCache().GetUrbanBlockCost( static_cast< float >( path_.GetUnitMajorWeight() ), from, to );
    return 0.;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathfinderRule::GetObjectsCost
// Created: NLD 2006-01-31
// -----------------------------------------------------------------------------
double DEC_Agent_PathfinderRule::GetObjectsCost( const MT_Vector2D& from, const MT_Vector2D& to, const TerrainData& nToTerrainType, const TerrainData& nLinkTerrainType, double& rSpeed ) const
{
    // default cost : outside all objects
    double rObjectCost = path_.GetCostOutsideOfAllObjects();
    const DEC_Agent_Path::T_PathKnowledgeObjectByTypesVector& knowledgesByTypes = path_.GetPathKnowledgeObjects();
    for( auto itType = knowledgesByTypes.begin(); itType != knowledgesByTypes.end(); ++itType )
    {
        bool bInsideObjectType = false;
        const DEC_Agent_Path::T_PathKnowledgeObjectVector& knowledges = *itType;
        for( auto itKnowledge = knowledges.begin(); itKnowledge != knowledges.end(); ++itKnowledge )
        {
            double rCurrentObjectCost = ( *itKnowledge )->ComputeCost( from, to, nToTerrainType, nLinkTerrainType, path_.GetUnitMajorWeight() );
            if( rCurrentObjectCost != std::numeric_limits< double >::min()  )
            {
                if( !bInsideObjectType )
                {
                    rObjectCost -= ( *itKnowledge )->GetCostOut();
                    bInsideObjectType = true;
                }
                if( rCurrentObjectCost < 0. ) // Impossible move (for example destroyed bridge)
                    return rCurrentObjectCost;
                rObjectCost += rCurrentObjectCost;
                if( ( *itKnowledge )->HasAgentMaxSpeedMultiplier() )
                    rSpeed = path_.GetUnitSpeeds().GetMaxSpeed() * ( *itKnowledge )->GetAgentMaxSpeedMultiplier();
            }
        }
    }
    return rObjectCost;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathfinderRule::GetEnemiesCost
// Created: NLD 2006-01-31
// -----------------------------------------------------------------------------
inline
double DEC_Agent_PathfinderRule::GetEnemiesCost( const MT_Vector2D& from, const MT_Vector2D& to ) const
{
    assert( path_.GetPathClass().AvoidEnemies() || path_.GetPathKnowledgeAgents().empty() );
    double rEnemyCost = 0.;
    const MT_Line lineLink( from, to );
    const DEC_Path_KnowledgeAgent::BoundingBox box( from, to );
    for( auto it = path_.GetPathKnowledgeAgents().begin(); it != path_.GetPathKnowledgeAgents().end(); ++it )
    {
        double rCurrentEnemyCost = it->ComputeCost( lineLink, box );
        if( rCurrentEnemyCost < 0. ) // Impossible move (for example destroyed bridge)
            return rCurrentEnemyCost;
        rEnemyCost += rCurrentEnemyCost;
    }
    if( rEnemyCost > rEnemyMaximumCost_ )
        rEnemyCost = rEnemyMaximumCost_;
    return rEnemyCost;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathfinderRule::GetPopulationsCost
// Created: SBO 2006-02-23
// -----------------------------------------------------------------------------
double DEC_Agent_PathfinderRule::GetPopulationsCost( const MT_Vector2D& to ) const
{
    assert( path_.GetPathClass().HandlePopulations() || path_.GetPathKnowledgePopulations().empty() );
    double rCost = 0.;
    for( auto it = path_.GetPathKnowledgePopulations().begin(); it != path_.GetPathKnowledgePopulations().end(); ++it )
    {
        double rCurrentCost = (*it)->ComputeCost( to );
        if( rCurrentCost < 0. ) // Impossible move
            return rCurrentCost;
        rCost += rCurrentCost;
    }
    if( rCost > rPopulationMaximumCost_ )
        rCost = rPopulationMaximumCost_;
    return rCost;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathfinderRule::GetAltitudeCost
// Created: NLD 2006-01-31
// -----------------------------------------------------------------------------
double DEC_Agent_PathfinderRule::GetAltitudeCost( double rAltitudeTo ) const
{
    if( rAltitudeCostPerMeter_ > 0 )
        return ( rMaxAltitude_ - rAltitudeTo ) * rAltitudeCostPerMeter_;
    else if( rAltitudeCostPerMeter_ < 0 )
        return ( rAltitudeTo - rMinAltitude_ ) * - rAltitudeCostPerMeter_;
    return 0.;
}

//#define DEBUG_IMPOSSIBLE_PATHFIND

#ifdef DEBUG_IMPOSSIBLE_PATHFIND

#include "MT_Tools/MT_Logger.h"

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

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathfinderRule::GetCost
// Created: AGE 2005-03-08
// -----------------------------------------------------------------------------
double DEC_Agent_PathfinderRule::GetCost( const MT_Vector2D& from, const MT_Vector2D& to, const TerrainData& nToTerrainType, const TerrainData& nLinkTerrainType )
{
    if( ! world_.IsValidPosition( to ) )
        return IMPOSSIBLE_DESTINATION( "Out of world" );

    // speed
    double rSpeed = path_.GetUnitSpeeds().GetMaxSpeed( nLinkTerrainType );
    if( rSpeed <= 0 )
        return IMPOSSIBLE_WAY( "Speeds on terrain" );

    if( ! path_.GetUnitSpeeds().IsPassable( nToTerrainType ) )
        return IMPOSSIBLE_DESTINATION( "Terrain type" );

    if( SplitOnMajorGridLines( static_cast< int32_t >( altitudeData_.GetCellSize() ), from, to,
        [&]( MT_Vector2D from, MT_Vector2D to ) -> bool
        {
            const double delta = altitudeData_.GetAltitude( to ) - altitudeData_.GetAltitude( from );
            // SLI 2013-12-11: beware of the square in the formula, negative slope (delta)
            // becomes positive and prevent vehicule to take a segment with a downslope superior than maxSquareSlope
            return delta * delta > from.SquareDistance( to ) * rMaxSlope_ * rMaxSlope_;
        } ) )
        return IMPOSSIBLE_WAY( "Slope" );

    if( rSlopeDeceleration_ != 0 )
    {
        SlopeSpeedModifier modifier;
        SplitOnMajorGridLines( static_cast< int32_t >( altitudeData_.GetCellSize() ), from, to,
            [&]( MT_Vector2D from, MT_Vector2D to )
            {
                return modifier.ComputeLocalSlope( altitudeData_, from, to );
            } );
        modifier.ModifySpeed( rSpeed, rSlopeDeceleration_, rMaxSlope_, to );
        if( rSpeed == 0 )
            return IMPOSSIBLE_WAY( "Speed on slope == 0" );
    }

    // Cost computation taken various dynamic terrain items into account
    double rDynamicCost = 0.;

    // Altitude
    const double rAltitudeTo   = altitudeData_.GetAltitude( to );
    const double rAltitudeCost = GetAltitudeCost( rAltitudeTo );
    rDynamicCost += rAltitudeCost;

    // Fuseaux
    const double rFuseauxCost = GetFuseauxCost( from, to );
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
    const double rUrbanBlockCost = GetUrbanBlockCost( from, to );
    if( rUrbanBlockCost < 0. )
        return IMPOSSIBLE_WAY( "Urban" );
    rDynamicCost += rUrbanBlockCost;

    // objects
    const double rObjectsCost = GetObjectsCost( from, to, nToTerrainType, nLinkTerrainType, rSpeed );
    if( rObjectsCost < 0 || rSpeed <= 0. )
        return IMPOSSIBLE_WAY( "Objects" );
    rDynamicCost += rObjectsCost;

    // enemies
    const double rEnemiesCost = GetEnemiesCost( from, to );
    if( rEnemiesCost < 0 )
        return IMPOSSIBLE_WAY( "Enemies" );
    rDynamicCost += rEnemiesCost;

    // populations
    const double rPopulationsCost = GetPopulationsCost( to );
    if( rPopulationsCost < 0 )
        return IMPOSSIBLE_WAY( "Populations" );
    rDynamicCost += rPopulationsCost;

    const double rDistance = from.Distance( to );
    const double rBaseCost = bShort_ ? rDistance : ( rDistance / rSpeed );
    return rBaseCost * ( 1 + rDynamicCost );
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathfinderRule::GetFuseauxCost
// Created: NLD 2006-01-31
// -----------------------------------------------------------------------------
double DEC_Agent_PathfinderRule::GetFuseauxCost( const MT_Vector2D& from, const MT_Vector2D& to ) const
{
    double rCost = 0.;
    // Vérification si le link est dans le fuseau de l'agent
    {
        const double rFuseauCost = pFuseau_ ? pFuseau_->GetCost( from, to, rMaximumFuseauDistance_, rFuseauCostPerMeterOut_, rComfortFuseauDistance_, rFuseauCostPerMeterIn_ ) : 0;
        if( rFuseauCost < 0 )
            return IMPOSSIBLE_WAY( "Fuseau agent" );
        rCost += rFuseauCost;
    }
    // $$$$ AGE 2005-06-24: Going out of the automate fuseau is a no-no. Avoid precision issues
    {
        const double rAutomateFuseauCost = pAutomateFuseau_ ? pAutomateFuseau_->GetCost( from, to, rMaximumAutomataFuseauDistance_, rAutomataFuseauCostPerMeterOut_, 0, 0 ) : 0;
        if( rAutomateFuseauCost < 0 )
            return IMPOSSIBLE_WAY( "Fuseau automate" );
        rCost += rAutomateFuseauCost;
    }
    return rCost;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathfinderRule::EvaluateCost
// Created: AGE 2005-03-08
// -----------------------------------------------------------------------------
float DEC_Agent_PathfinderRule::EvaluateCost( const geometry::Point2f& from, const geometry::Point2f& to )
{
    const float rDistance = from.Distance( to );
    if( rMaxSpeed_ != 0 )
        return bShort_ ? rDistance : ( rDistance / rMaxSpeed_ );
    return std::numeric_limits< float >::max();
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathfinderRule::GetCost
// Created: AGE 2005-03-08
// -----------------------------------------------------------------------------
float DEC_Agent_PathfinderRule::GetCost( const geometry::Point2f& from, const geometry::Point2f& to, const TerrainData& terrainTo, const TerrainData& terrainBetween )
{
    return float( GetCost( MT_Vector2D( from.X(), from.Y() ),
                           MT_Vector2D( to.X(), to.Y() ),
                           terrainTo,
                           terrainBetween ) );
}

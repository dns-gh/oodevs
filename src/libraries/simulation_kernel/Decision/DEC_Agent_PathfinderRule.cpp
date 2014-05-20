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
#include "DEC_AgentContext_ABC.h"
#include "DEC_Agent_PathClass.h"
#include "DEC_Path_KnowledgeObject_ABC.h"
#include "DEC_Path_KnowledgePopulation.h"
#include "DEC_Path_KnowledgeAgent.h"
#include "MIL_AgentServer.h"
#include "SlopeSpeedModifier.h"
#include "Entities/Agents/Units/PHY_Speeds.h"
#include "Entities/Orders/MIL_Fuseau.h"
#include "simulation_terrain/TER_World.h"
#include "Tools/MIL_Tools.h"
#include "Urban/MIL_UrbanCache.h"
#include "Meteo/RawVisionData/PHY_RawVisionData.h"
#include "Meteo/RawVisionData/Elevation.h"
#include "Meteo/PHY_MeteoDataManager.h"

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathfinderRule constructor
// Created: AGE 2005-03-23
// -----------------------------------------------------------------------------
DEC_Agent_PathfinderRule::DEC_Agent_PathfinderRule( const boost::shared_ptr< DEC_AgentContext_ABC >& context, const MT_Vector2D& from, const MT_Vector2D& to )
    : context_                       ( context )
    , world_                         ( TER_World::GetWorld() )
    , altitudeData_                  ( MIL_AgentServer::GetWorkspace().GetMeteoDataManager().GetRawVisionData() )
    , rMaxSpeed_                     ( float( context->GetUnitSpeeds().GetMaxSpeed() ) * 1.1f )
    , bShort_                        ( context->GetPathClass().IsShort() )
    , avoidedTerrain_                ( context->GetPathClass().GetAvoidedTerrain() )
    , rAvoidedTerrainCost_           ( avoidedTerrain_ == TerrainData() ? 0. : context->GetPathClass().GetAvoidedTerrainCost() )
    , preferedTerrain_               ( context->GetPathClass().GetPreferedTerrain() )
    , rPreferedTerrainCost_          ( preferedTerrain_ == TerrainData() ? 0. : context->GetPathClass().GetPreferedTerrainCost() )
    , rMinAltitude_                  ( altitudeData_.GetMinAltitude() )
    , rMaxAltitude_                  ( altitudeData_.GetMaxAltitude() )
    , rAltitudeCostPerMeter_         ( context->GetPathClass().GetAltitudePreference() )
    , rMaxSlope_                     ( context->GetUnitMaxSlope() )
    , rSlopeDeceleration_            ( context->GetUnitSlopeDeceleration() )
    , pFuseau_                       ( 0 )
    , rComfortFuseauDistance_        ( context->GetPathClass().GetComfortFuseauDistance() )
    , rFuseauCostPerMeterOut_        ( context->GetPathClass().GetFuseauCostPerMeterOut() )
    , rFuseauCostPerMeterIn_         ( context->GetPathClass().GetFuseauCostPerMeterIn () )
    , rMaximumFuseauDistance_        ( 0. ) // Initialized below
    , pAutomateFuseau_               ( 0 )
    , rAutomataFuseauCostPerMeterOut_( context->GetPathClass().GetAutomateFuseauCostPerMeterOut() )
    , rMaximumAutomataFuseauDistance_( 0. ) // Initialized below
    , dangerDirection_               ( context->GetDirDanger() )
    , rDangerDirectionBaseCost_      ( context->GetPathClass().GetDangerDirectionBaseCost  () )
    , rDangerDirectionLinearCost_    ( context->GetPathClass().GetDangerDirectionLinearCost() )
    , dangerPoint_                   ( DotProduct( dangerDirection_, from ) > DotProduct( dangerDirection_, to ) ? from : to )
    , rEnemyMaximumCost_             ( context->GetPathClass().GetEnemyMaximumCost() )
    , rPopulationMaximumCost_        ( context->GetPathClass().GetPopulationMaximumCost() )
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
// Name: DEC_Agent_PathfinderRule::InitializeFuseauData
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
void DEC_Agent_PathfinderRule::InitializeFuseauData( const MT_Vector2D& from, const MT_Vector2D& to )
{
    assert( pFuseau_ == 0 );
    if( context_->GetFuseau().IsNull() )
        return;
    pFuseau_ = &context_->GetFuseau();
    rMaximumFuseauDistance_ = context_->GetPathClass().GetMaximumFuseauDistance();
    if( !pFuseau_->IsInside( from ) )
        rMaximumFuseauDistance_ += pFuseau_->Distance( from ) * 1.3; // $$$$ AGE 2005-06-08:
    if( !context_->GetAutomataFuseau().IsNull() ) // I have a higher fuseau
        rMaximumFuseauDistance_ = std::max( rMaximumFuseauDistance_, context_->GetPathClass().GetMaximumFuseauDistanceWithAutomata() );
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
    if( context_->GetAutomataFuseau().IsNull() )
        return;
    pAutomateFuseau_ = &context_->GetAutomataFuseau();
    rMaximumAutomataFuseauDistance_ = context_->GetPathClass().GetMaximumAutomataFuseauDistance();
    if( !pAutomateFuseau_->IsInside( from ) )
        rMaximumAutomataFuseauDistance_ += pAutomateFuseau_->Distance( from ) * 1.3; // $$$$ AGE
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
    if( MIL_AgentServer::IsInitialized() && !context_->GetPathClass().IsFlying() )
        return MIL_AgentServer::GetWorkspace().GetUrbanCache().GetUrbanBlockCost( static_cast< float >( context_->GetUnitMajorWeight() ), from, to );
    return 0.;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathfinderRule::GetObjectsCost
// Created: NLD 2006-01-31
// -----------------------------------------------------------------------------
double DEC_Agent_PathfinderRule::GetObjectsCost( const MT_Vector2D& from, const MT_Vector2D& to, const TerrainData& nToTerrainType, const TerrainData& nLinkTerrainType, double& rSpeed ) const
{
    // default cost : outside all objects
    double rObjectCost = context_->GetCostOutsideOfAllObjects();
    const auto& knowledgesByTypes = context_->GetPathKnowledgeObjects();
    for( auto itType = knowledgesByTypes.begin(); itType != knowledgesByTypes.end(); ++itType )
    {
        bool bInsideObjectType = false;
        const auto& knowledges = *itType;
        for( auto itKnowledge = knowledges.begin(); itKnowledge != knowledges.end(); ++itKnowledge )
        {
            double rCurrentObjectCost = ( *itKnowledge )->ComputeCost( from, to, nToTerrainType, nLinkTerrainType, context_->GetUnitMajorWeight() );
            if( rCurrentObjectCost != std::numeric_limits< double >::min() )
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
                    rSpeed = context_->GetUnitSpeeds().GetMaxSpeed() * ( *itKnowledge )->GetAgentMaxSpeedMultiplier();
            }
        }
    }
    return rObjectCost;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathfinderRule::GetEnemiesCost
// Created: NLD 2006-01-31
// -----------------------------------------------------------------------------
double DEC_Agent_PathfinderRule::GetEnemiesCost( const MT_Vector2D& from, const MT_Vector2D& to ) const
{
    double rEnemyCost = 0.;
    const MT_Line lineLink( from, to );
    const DEC_Path_KnowledgeAgent::BoundingBox box( from, to );
    for( auto it = context_->GetPathKnowledgeAgents().begin(); it != context_->GetPathKnowledgeAgents().end(); ++it )
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
    double rCost = 0.;
    for( auto it = context_->GetPathKnowledgePopulations().begin(); it != context_->GetPathKnowledgePopulations().end(); ++it )
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

#define LOG_REASON( message )                           \
    if( reason )                                        \
        *reason << message << std::endl;

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathfinderRule::GetCost
// Created: AGE 2005-03-08
// -----------------------------------------------------------------------------
double DEC_Agent_PathfinderRule::GetCost( const MT_Vector2D& from, const MT_Vector2D& to, const TerrainData& nToTerrainType, const TerrainData& nLinkTerrainType, std::ostream* reason )
{
    if( ! world_.IsValidPosition( to ) )
    {
        LOG_REASON( "no context: out of world" );
        return -1;
    }

    // speed
    double rSpeed = context_->GetUnitSpeeds().GetMaxSpeed( nLinkTerrainType );
    if( rSpeed <= 0 )
    {
        LOG_REASON( "no context: speed on " << nLinkTerrainType.DumpToString()
                << " = " << rSpeed );
        return -1;
    }

    if( ! context_->GetUnitSpeeds().IsPassable( nToTerrainType ) )
    {
        LOG_REASON( "no context: cannot cross terrain" );
        return -1;
    }

    if( SplitOnMajorGridLines( static_cast< int32_t >( altitudeData_.GetCellSize() ), from, to,
        [&]( MT_Vector2D from, MT_Vector2D to ) -> bool
        {
            const double delta = altitudeData_.GetAltitude( to ) - altitudeData_.GetAltitude( from );
            // SLI 2013-12-11: beware of the square in the formula, negative slope (delta)
            // becomes positive and prevent vehicule to take a segment with a downslope superior than maxSquareSlope
            return delta * delta > from.SquareDistance( to ) * rMaxSlope_ * rMaxSlope_;
        } ) )
    {
        LOG_REASON( "no context: slope is too steep" );
        return -1;
    }

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
        {
            LOG_REASON( "no context: speed on slope is null" );
            return -1;
        }
    }

    // Cost computation taken various dynamic terrain items into account
    double rDynamicCost = 0.;

    // Altitude
    const double rAltitudeTo   = altitudeData_.GetAltitude( to );
    const double rAltitudeCost = GetAltitudeCost( rAltitudeTo );
    rDynamicCost += rAltitudeCost;

    // Fuseaux
    const double rFuseauxCost = GetFuseauxCost( from, to, reason );
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
    {
        LOG_REASON( "no context: urban block cost: " << rUrbanBlockCost );
        return -1;
    }
    rDynamicCost += rUrbanBlockCost;

    // objects
    const double rObjectsCost = GetObjectsCost( from, to, nToTerrainType, nLinkTerrainType, rSpeed );
    if( rObjectsCost < 0 || rSpeed <= 0. )
    {
        LOG_REASON( "no context: objects cost: " << rObjectsCost << ", speed: " << rSpeed );
        return -1;
    }
    rDynamicCost += rObjectsCost;

    // enemies
    const double rEnemiesCost = GetEnemiesCost( from, to );
    if( rEnemiesCost < 0 )
    {
        LOG_REASON( "no context: enemies cost: " << rEnemiesCost );
        return -1;
    }
    rDynamicCost += rEnemiesCost;

    // populations
    const double rPopulationsCost = GetPopulationsCost( to );
    if( rPopulationsCost < 0 )
    {
        LOG_REASON( "no context: populations cost: " << rPopulationsCost );
        return -1;
    }
    rDynamicCost += rPopulationsCost;

    const double rDistance = from.Distance( to );
    const double rBaseCost = bShort_ ? rDistance : ( rDistance / rSpeed );
    return rBaseCost * ( 1 + rDynamicCost );
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathfinderRule::GetFuseauxCost
// Created: NLD 2006-01-31
// -----------------------------------------------------------------------------
double DEC_Agent_PathfinderRule::GetFuseauxCost( const MT_Vector2D& from, const MT_Vector2D& to,
       std::ostream* reason ) const
{
    double rCost = 0.;
    // Vérification si le link est dans le fuseau de l'agent
    {
        const double rFuseauCost = pFuseau_ ? pFuseau_->GetCost( from, to, rMaximumFuseauDistance_, rFuseauCostPerMeterOut_, rComfortFuseauDistance_, rFuseauCostPerMeterIn_ ) : 0;
        if( rFuseauCost < 0 )
        {
            LOG_REASON( "no context: unit limits" );
            return -1;
        }
        rCost += rFuseauCost;
    }
    // $$$$ AGE 2005-06-24: Going out of the automate fuseau is a no-no. Avoid precision issues
    {
        const double rAutomateFuseauCost = pAutomateFuseau_ ? pAutomateFuseau_->GetCost( from, to, rMaximumAutomataFuseauDistance_, rAutomataFuseauCostPerMeterOut_, 0, 0 ) : 0;
        if( rAutomateFuseauCost < 0 )
        {
            LOG_REASON( "no context: automat limits" );
            return -1;
        }
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
float DEC_Agent_PathfinderRule::GetCost( const geometry::Point2f& from, const geometry::Point2f& to, const TerrainData& terrainTo, const TerrainData& terrainBetween, std::ostream* reason )
{
    return float( GetCost( MT_Vector2D( from.X(), from.Y() ),
                           MT_Vector2D( to.X(), to.Y() ),
                           terrainTo,
                           terrainBetween,
                           reason ) );
}

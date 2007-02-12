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
#include "simulation_terrain/TER_World.h"
#include "Tools/MIL_Tools.h"
#include "Entities/Agents/Units/PHY_Speeds.h"
#include "Meteo/RawVisionData/PHY_RawVisionData.h"
#include "Meteo/PHY_MeteoDataManager.h"

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathfinderRule::InitializeFuseauData
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
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
        rMaximumFuseauDistance_ = std::numeric_limits< MT_Float >::max();
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathfinderRule::InitializeAutomateFuseauData
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
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
    : path_                          ( path )
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
    InitializeFuseauData        ( from, to );
    InitializeAutomateFuseauData( from );
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathfinderRule destructor
// Created: AGE 2005-03-08
// -----------------------------------------------------------------------------
DEC_Agent_PathfinderRule::~DEC_Agent_PathfinderRule()
{
    //NOTHING
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathfinderRule::GetTerrainCost
// Created: AGE 2005-03-25
// -----------------------------------------------------------------------------
inline
MT_Float DEC_Agent_PathfinderRule::GetTerrainCost( const TerrainData& data ) const
{
    MT_Float rDynamicCost = 0;
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
inline
MT_Float DEC_Agent_PathfinderRule::GetDangerDirectionCost( const MT_Vector2D& to ) const
{
    // direction dangereuse
    if( !dangerDirection_.IsZero() )
    {
        const MT_Float dp = DotProduct( to - dangerPoint_, dangerDirection_ );
        if( dp > 0 ) // we are beyond our danger point
            return ( rDangerDirectionBaseCost_ + dp * rDangerDirectionLinearCost_ );
    }
    return 0.;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathfinderRule::GetObjectsCost
// Created: NLD 2006-01-31
// -----------------------------------------------------------------------------
inline
MT_Float DEC_Agent_PathfinderRule::GetObjectsCost( const MT_Vector2D& from, const MT_Vector2D& to, const TerrainData& nToTerrainType, const TerrainData& nLinkTerrainType ) const
{  
    // default cost : outside all objects
    MT_Float rObjectCost = path_.GetCostOutsideOfAllObjects();

    const DEC_Agent_Path::T_PathKnowledgeObjectByTypesVector& knowledgesByTypes = path_.GetPathKnowledgeObjects();
    for( DEC_Agent_Path::CIT_PathKnowledgeObjectByTypesVector itType = knowledgesByTypes.begin(); itType != knowledgesByTypes.end(); ++itType )
    {
        bool bInsideObjectType = false;
        const DEC_Agent_Path::T_PathKnowledgeObjectVector& knowledges = *itType;
        for( DEC_Agent_Path::CIT_PathKnowledgeObjectVector itKnowledge = knowledges.begin(); itKnowledge != knowledges.end(); ++itKnowledge )
        {
            MT_Float rCurrentObjectCost = itKnowledge->ComputeCost( from, to, nToTerrainType, nLinkTerrainType );
            if( rCurrentObjectCost != std::numeric_limits< MT_Float >::min()  )
            {
                if( !bInsideObjectType )
                {
                    rObjectCost -= itKnowledge->GetCostOut();
                    bInsideObjectType = true;
                }
                if( rCurrentObjectCost < 0. ) // Impossible move (for example destroyed bridge)
                    return rCurrentObjectCost;
                rObjectCost += rCurrentObjectCost;
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
MT_Float DEC_Agent_PathfinderRule::GetEnemiesCost( const MT_Vector2D& from, const MT_Vector2D& to, const TerrainData& nToTerrainType, const TerrainData& nLinkTerrainType ) const
{
    assert( ( !path_.GetPathClass().AvoidEnemies() && path_.GetPathKnowledgeAgents().empty() ) || path_.GetPathClass().AvoidEnemies() );
    MT_Float rEnemyCost = 0.;
    for( DEC_Agent_Path::CIT_PathKnowledgeAgentVector it = path_.GetPathKnowledgeAgents().begin(); it != path_.GetPathKnowledgeAgents().end(); ++it )
    {
        MT_Float rCurrentEnemyCost = it->ComputeCost( from, to, nToTerrainType, nLinkTerrainType );
        if( rCurrentEnemyCost < 0. ) // Impossible move (for example destroyed bridge)
            return rCurrentEnemyCost;
        rEnemyCost += rCurrentEnemyCost;
    }
    if( rEnemyCost > rEnemyMaximumCost_ )
        rEnemyCost = rEnemyMaximumCost_;
    return rEnemyCost;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathfinderRule::GetFuseauxCost
// Created: NLD 2006-01-31
// -----------------------------------------------------------------------------
inline
MT_Float DEC_Agent_PathfinderRule::GetFuseauxCost( const MT_Vector2D& from, const MT_Vector2D& to ) const
{
    MT_Float rCost = 0.;
    // Vérification si le link est dans le fuseau de l'agent
    {
        const MT_Float rFuseauCost = pFuseau_ ? pFuseau_->GetCost( from, to, rMaximumFuseauDistance_, rFuseauCostPerMeterOut_, rComfortFuseauDistance_, rFuseauCostPerMeterIn_ ) : 0;
        if( rFuseauCost < 0 )
            return rFuseauCost;
        rCost += rFuseauCost;
    }
    
    // $$$$ AGE 2005-06-24: Going out of the automate fuseau is a no-no. Avoid precision issues
    {
        const MT_Float rAutomateFuseauCost = pAutomateFuseau_ ? pAutomateFuseau_->GetCost( from, to, rMaximumAutomataFuseauDistance_, rAutomataFuseauCostPerMeterOut_, 0, 0 ) : 0;
        if( rAutomateFuseauCost < 0 )
            return rAutomateFuseauCost;
        rCost += rAutomateFuseauCost;
    }
    return rCost;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathfinderRule::GetPopulationsCost
// Created: SBO 2006-02-23
// -----------------------------------------------------------------------------
MT_Float DEC_Agent_PathfinderRule::GetPopulationsCost( const MT_Vector2D& from, const MT_Vector2D& to, const TerrainData& nToTerrainType, const TerrainData& nLinkTerrainType ) const
{
    assert( ( !path_.GetPathClass().HandlePopulations() && path_.GetPathKnowledgePopulations().empty() ) || path_.GetPathClass().HandlePopulations() );
    MT_Float rCost = 0.;
    for( DEC_Agent_Path::CIT_PathKnowledgePopulationVector it = path_.GetPathKnowledgePopulations().begin(); it != path_.GetPathKnowledgePopulations().end(); ++it )
    {
        MT_Float rCurrentCost = it->ComputeCost( from, to, nToTerrainType, nLinkTerrainType );
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
inline
MT_Float DEC_Agent_PathfinderRule::GetAltitudeCost( MT_Float rAltitudeTo ) const
{
    if( rAltitudeCostPerMeter_ > 0 )
        return ( rMaxAltitude_ - rAltitudeTo ) * rAltitudeCostPerMeter_;
    else if( rAltitudeCostPerMeter_ < 0 )
        return ( rAltitudeTo - rMinAltitude_ ) * - rAltitudeCostPerMeter_;
    return 0.;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathfinderRule::GetCost
// Created: AGE 2005-03-08
// -----------------------------------------------------------------------------
MT_Float DEC_Agent_PathfinderRule::GetCost( const MT_Vector2D& from, const MT_Vector2D& to, const TerrainData& nToTerrainType, const TerrainData& nLinkTerrainType )
{
    if( ! world_.IsValidPosition( to ) )
        return -1.;

    // vitesse
    const MT_Float rSpeed = path_.GetUnitSpeeds().GetMaxSpeed( nLinkTerrainType );
    if( rSpeed <= 0. )
        return -1;

    if( ! path_.GetUnitSpeeds().IsPassable( nToTerrainType ) )
        return -1;
    
    const MT_Float rDistance     = from.Distance( to );
    const MT_Float rAltitudeFrom = altitudeData_.GetAltitude( from );
    const MT_Float rAltitudeTo   = altitudeData_.GetAltitude( to );
    {
        const MT_Float rDelta          = rAltitudeTo - rAltitudeFrom;
        const MT_Float rGroundDistance = sqrt( rDelta * rDelta + rDistance * rDistance );
        const MT_Float rSlope          = rGroundDistance > 0 ? rDelta / rGroundDistance : 0;
        if( rSlope > rMaxSlope_ )
            return -1;
    }

    // Calcul du cout dû aux éléments dynamiques du terrain
    // (ennemis, objets du génie, fuseaux, flot..)
    MT_Float rDynamicCost = 0.;

    // Altitude
    rDynamicCost += GetAltitudeCost( rAltitudeTo );

    // Fuseaux
    const MT_Float rFuseauxCost = GetFuseauxCost( from, to );
    if( rFuseauxCost < 0. )
        return rFuseauxCost;
    rDynamicCost += rFuseauxCost;
    
    // types de terrain
    rDynamicCost += GetTerrainCost( nLinkTerrainType );

    // direction dangereuse
    rDynamicCost += GetDangerDirectionCost( to );

    // objects
    const MT_Float rObjectsCost = GetObjectsCost( from, to, nToTerrainType, nLinkTerrainType );
    if( rObjectsCost < 0 )
        return rObjectsCost;
    rDynamicCost += rObjectsCost;

    // ennemies
    const MT_Float rEnemiesCost = GetEnemiesCost( from, to, nToTerrainType, nLinkTerrainType );
    if( rEnemiesCost < 0 )
        return rEnemiesCost;
    rDynamicCost += rEnemiesCost;

    // populations
    const MT_Float rPopulationsCost = GetPopulationsCost( from, to, nToTerrainType, nLinkTerrainType );
    if( rPopulationsCost < 0 )
        return rPopulationsCost;
    rDynamicCost += rPopulationsCost;   

    const MT_Float rBaseCost = bShort_ ? rDistance : ( rDistance / rSpeed );
    return rBaseCost * ( 1 + rDynamicCost );
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathfinderRule::EvaluateCost
// Created: AGE 2005-03-08
// -----------------------------------------------------------------------------
float DEC_Agent_PathfinderRule::EvaluateCost( const geometry::Point2f& from, const geometry::Point2f& to )
{
    const float rDistance = from.Distance( to );
    return bShort_ ? rDistance : ( rDistance / rMaxSpeed_ );
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
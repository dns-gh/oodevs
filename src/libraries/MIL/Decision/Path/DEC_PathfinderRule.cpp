// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2005-03-08 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Path/DEC_PathfinderRule.cpp $
// $Author: Nld $
// $Modtime: 20/07/05 18:46 $
// $Revision: 10 $
// $Workfile: DEC_PathfinderRule.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"
#include "DEC_PathfinderRule.h"
#include "DEC_PathType.h"
#include "DEC_Path.h"
#include "TER/TER_World.h"
#include "Tools/MIL_Tools.h"
#include "Entities/Agents/Units/PHY_Speeds.h"
#include "Meteo/RawVisionData/PHY_RawVisionData.h"
#include "Meteo/PHY_MeteoDataManager.h"

// -----------------------------------------------------------------------------
// Name: DEC_PathfinderRule constructor
// Created: AGE 2005-03-23
// -----------------------------------------------------------------------------
DEC_PathfinderRule::DEC_PathfinderRule( const DEC_Path& path, const MT_Vector2D& from, const MT_Vector2D& to, bool bShort )
    : world_                     ( TER_World::GetWorld() )
    , altitudeData_              ( MIL_AgentServer::GetWorkspace().GetMeteoDataManager().GetRawVisionData() )
    , speeds_                    ( path.GetUnitSpeeds() )
    , pFuseau_                   ( 0 ) 
    , pAutomateFuseau_           ( 0 )
    , dangerDirection_           ( path.GetDirDanger() )
    , rDangerDirectionBaseCost_  ( 1 )
    , rDangerDirectionLinearCost_( 0.01 )
    , rMaxSpeed_                 ( float( speeds_.GetMaxSpeed() ) * 1.1f )
    , bShort_                    ( bShort )
    , rMaximumFuseauDistance_    ( 10 )
    , rComfortFuseauDistance_    ( 500 )
    , rFuseauCostPerMeterOut_    ( 1 )
    , rFuseauCostPerMeterIn_     ( 1e-2 )
    , rMaxEnemyCost_             ( 50 )
    , rAvoidCost_                ( 0 )
    , rPreferCost_               ( 0 )
    , rMinAltitude_              ( MIL_AgentServer::GetWorkspace().GetMeteoDataManager().GetRawVisionData().GetMinAltitude() )
    , rMaxAltitude_              ( MIL_AgentServer::GetWorkspace().GetMeteoDataManager().GetRawVisionData().GetMaxAltitude() )
    , rAltitudeCostPerMeter_     ( 1e-3 )
    , rMaxSlope_                 ( path.GetUnitMaxSlope() )
{
    if( ! path.GetFuseau().IsNull() )
        pFuseau_ = & path.GetFuseau();
    if( ! path.GetAutomataFuseau().IsNull() 
       && path.GetAutomataFuseau().IsInside( from )
       && path.GetAutomataFuseau().IsInside( to ) )
        pAutomateFuseau_ = & path.GetAutomataFuseau();
    dangerPoint_ = DotProduct( dangerDirection_, from ) > DotProduct( dangerDirection_, to ) ? from : to;
    agents_ .reserve( 10 );
    objects_.reserve( 10 );
}

// -----------------------------------------------------------------------------
// Name: DEC_PathfinderRule destructor
// Created: AGE 2005-03-08
// -----------------------------------------------------------------------------
DEC_PathfinderRule::~DEC_PathfinderRule()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_PathfinderRule::AddEnemyKnowledge
// Created: AGE 2005-03-25
// -----------------------------------------------------------------------------
void DEC_PathfinderRule::AddEnemyKnowledge( const DEC_Path_KnowledgeAgent & enemy, MT_Float rCostOnTarget, MT_Float rCostAtSecurityRange )
{
    agents_.push_back( enemy );
    agents_.back().SetCosts( rCostOnTarget, rCostAtSecurityRange );
}

// -----------------------------------------------------------------------------
// Name: DEC_PathfinderRule::AddObjectKnowledge
// Created: AGE 2005-03-25
// -----------------------------------------------------------------------------
void DEC_PathfinderRule::AddObjectKnowledge( const DEC_Path_KnowledgeObject& object, MT_Float rCost )
{
    objects_.push_back( object );
    objects_.back().SetCost( rCost );
}

// -----------------------------------------------------------------------------
// Name: DEC_PathfinderRule::SetMaximumEnemyCost
// Created: AGE 2005-08-04
// -----------------------------------------------------------------------------
void DEC_PathfinderRule::SetMaximumEnemyCost( MT_Float rNewCost )
{
    rMaxEnemyCost_ = rNewCost;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathfinderRule::SetDangerDirectionCosts
// Created: AGE 2005-06-24
// -----------------------------------------------------------------------------
void DEC_PathfinderRule::SetDangerDirectionCosts( MT_Float rBaseCost, MT_Float rLinearCost )
{
    rDangerDirectionBaseCost_ = rBaseCost;
    rDangerDirectionLinearCost_ = rLinearCost;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathfinderRule::SetPreferedTerrain
// Created: AGE 2005-08-04
// -----------------------------------------------------------------------------
void DEC_PathfinderRule::SetPreferedTerrain( const TerrainData& data, MT_Float rCostOut )
{
    prefer_      = data;
    if( data == TerrainData() )
        rCostOut = 0;
    rPreferCost_ = rCostOut;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathfinderRule::SetAvoidedTerrain
// Created: AGE 2005-08-04
// -----------------------------------------------------------------------------
void DEC_PathfinderRule::SetAvoidedTerrain( const TerrainData& data, MT_Float rCostIn )
{
    avoid_      = data;
    if( data == TerrainData() )
        rCostIn = 0;
    rAvoidCost_ = rCostIn;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathfinderRule::SetAltitudePreference
// Created: AGE 2005-08-04
// -----------------------------------------------------------------------------
void DEC_PathfinderRule::SetAltitudePreference( MT_Float rCostPerMeter )
{
    rAltitudeCostPerMeter_ = rCostPerMeter;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathfinderRule::SetFuseauCosts
// Created: AGE 2005-08-04
// -----------------------------------------------------------------------------
void DEC_PathfinderRule::SetFuseauCosts( MT_Float rMaxDistanceOut, MT_Float rCostPerMeterOut, MT_Float rComfortDistanceIn, MT_Float rCostPerMeterIn )
{
    rMaximumFuseauDistance_ = rMaxDistanceOut;
    rComfortFuseauDistance_ = rComfortDistanceIn;
    rFuseauCostPerMeterOut_ = rCostPerMeterOut; 
    rFuseauCostPerMeterIn_  = rCostPerMeterIn;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathfinderRule::EvaluateCost
// Created: AGE 2005-03-08
// -----------------------------------------------------------------------------
float DEC_PathfinderRule::EvaluateCost( const geometry::Point2f& from, const geometry::Point2f& to )
{
    const float rDistance = from.Distance( to );
    return bShort_ ? rDistance : ( rDistance / rMaxSpeed_ );
}

// -----------------------------------------------------------------------------
// Name: DEC_PathfinderRule::GetCost
// Created: AGE 2005-03-08
// -----------------------------------------------------------------------------
float DEC_PathfinderRule::GetCost( const geometry::Point2f& from, const geometry::Point2f& to, const TerrainData& terrainTo, const TerrainData& terrainBetween )
{
    return float( GetCost( MT_Vector2D( from.X(), from.Y() ), 
                           MT_Vector2D( to.X(), to.Y() ),
                           terrainTo,
                           terrainBetween ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_PathfinderRule::GetCost
// Created: AGE 2005-03-08
// -----------------------------------------------------------------------------
MT_Float DEC_PathfinderRule::GetCost( const MT_Vector2D& from, const MT_Vector2D& to, const TerrainData& nToTerrainType, const TerrainData& nLinkTerrainType )
{
    if( ! world_.IsValidPosition( to ) )
        return -1.;

    // vitesse
    const MT_Float rSpeed = speeds_.GetMaxSpeed( nLinkTerrainType );
    if( rSpeed <= 0. )
        return -1;

    if( ! speeds_.IsPassable( nToTerrainType ) )
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

    if( rAltitudeCostPerMeter_ > 0 )
        rDynamicCost += ( rMaxAltitude_ - rAltitudeTo ) * rAltitudeCostPerMeter_;
    else if( rAltitudeCostPerMeter_ < 0 )
        rDynamicCost += ( rAltitudeTo - rMinAltitude_ ) * - rAltitudeCostPerMeter_;

    // Vérification si le link est dans le fuseau de l'agent
    MT_Float rFuseauCost = pFuseau_ ? pFuseau_->GetCost( from, to, rMaximumFuseauDistance_, rFuseauCostPerMeterOut_, rComfortFuseauDistance_, rFuseauCostPerMeterIn_ ) : 0;
    if( rFuseauCost < 0 )
        return -1;

    if( rFuseauCost > 0 )
    {
         // $$$$ AGE 2005-06-24: Going out of the automate fuseau is a no-no. Avoid precision issues
        const MT_Float rAutomateFuseauCost = pAutomateFuseau_ ? pAutomateFuseau_->GetCost( from, to, 10, 1, 0, 0 ) : 0;
        if( rAutomateFuseauCost < 0 )
            return rAutomateFuseauCost;
        rFuseauCost += rAutomateFuseauCost;
    }
    rDynamicCost += rFuseauCost;

    // types de terrain
    rDynamicCost += TerrainCost( nLinkTerrainType );

    // direction dangereuse
    if( ! dangerDirection_.IsZero() )
    {
        const MT_Float dp = DotProduct( to - dangerPoint_, dangerDirection_ );
        if( dp > 0 ) // we are beyond our danger point
            rDynamicCost += rDangerDirectionBaseCost_ + dp * rDangerDirectionLinearCost_;
    }

    {
        MT_Float rObjectCost = 0;
        for( DEC_Path::CIT_PathKnowledgeObjectVector itKnowledge = objects_.begin(); itKnowledge != objects_.end(); ++itKnowledge )
        {
            MT_Float rCurrentObjectCost = itKnowledge->ComputeCost( from, to, nToTerrainType, nLinkTerrainType );
            if( rCurrentObjectCost < 0. ) // Impossible move (for example destroyed bridge)
                return rCurrentObjectCost;
            rObjectCost += rCurrentObjectCost;
        }
        rDynamicCost += rObjectCost;
    }

    {
        MT_Float rEnemyCost = 0;
        for( DEC_Path::CIT_PathKnowledgeAgentVector itKnowledge = agents_.begin(); itKnowledge != agents_.end(); ++itKnowledge )
        {
            MT_Float rCurrentEnemyCost = itKnowledge->ComputeCost( from, to, nToTerrainType, nLinkTerrainType );
            if( rCurrentEnemyCost < 0. ) // Impossible move (for example destroyed bridge)
                return rCurrentEnemyCost;
            rEnemyCost += rCurrentEnemyCost;
        }
        if( rEnemyCost > rMaxEnemyCost_ )
            rEnemyCost = rMaxEnemyCost_;
        rDynamicCost += rEnemyCost;
    }

    const MT_Float rBaseCost = bShort_ ? rDistance : ( rDistance / rSpeed );
    return rBaseCost * ( 1 + rDynamicCost );
}

// -----------------------------------------------------------------------------
// Name: DEC_PathfinderRule::TerrainCost
// Created: AGE 2005-03-25
// -----------------------------------------------------------------------------
MT_Float DEC_PathfinderRule::TerrainCost( const TerrainData& data ) const
{
    MT_Float rDynamicCost = 0;
    if( rAvoidCost_ > 0 && avoid_.ContainsOne( data ) )
        rDynamicCost += rAvoidCost_;

    if( rPreferCost_ > 0 && ! prefer_.ContainsOne( data ) )
        rDynamicCost += rPreferCost_;
    return rDynamicCost;
}

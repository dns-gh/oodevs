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
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Path/DEC_Agent_PathfinderRule.cpp $
// $Author: Nld $
// $Modtime: 20/07/05 18:46 $
// $Revision: 10 $
// $Workfile: DEC_Agent_PathfinderRule.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"
#include "DEC_Agent_PathfinderRule.h"
#include "DEC_Agent_Path.h"
#include "Decision/Path/DEC_PathType.h"
#include "TER/TER_World.h"
#include "Tools/MIL_Tools.h"
#include "Entities/Agents/Units/PHY_Speeds.h"
#include "Meteo/RawVisionData/PHY_RawVisionData.h"
#include "Meteo/PHY_MeteoDataManager.h"

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathfinderRule constructor
// Created: AGE 2005-03-23
// -----------------------------------------------------------------------------
DEC_Agent_PathfinderRule::DEC_Agent_PathfinderRule( const PHY_Speeds& speeds, const MT_Vector2D& from, const MT_Vector2D& to, bool bShort )
    : TerrainRule_ABC            ()
    , world_                     ( TER_World::GetWorld() )
    , altitudeData_              ( MIL_AgentServer::GetWorkspace().GetMeteoDataManager().GetRawVisionData() )
    , speeds_                    ( speeds )
    , pFuseau_                   ( 0 ) 
    , pAutomateFuseau_           ( 0 )
    , dangerDirection_           ( )
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
    , rMaxSlope_                 ( std::numeric_limits<MT_Float>::infinity() )
{
    dangerPoint_ = DotProduct( dangerDirection_, from ) > DotProduct( dangerDirection_, to ) ? from : to;
    agents_ .reserve( 10 );
    objects_.reserve( 10 );
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathfinderRule destructor
// Created: AGE 2005-03-08
// -----------------------------------------------------------------------------
DEC_Agent_PathfinderRule::~DEC_Agent_PathfinderRule()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathfinderRule::AddEnemyKnowledge
// Created: AGE 2005-03-25
// -----------------------------------------------------------------------------
void DEC_Agent_PathfinderRule::AddEnemyKnowledge( const DEC_Path_KnowledgeAgent & enemy, MT_Float rCostOnTarget, MT_Float rCostAtSecurityRange )
{
    agents_.push_back( enemy );
    agents_.back().SetCosts( rCostOnTarget, rCostAtSecurityRange );
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathfinderRule::AddObjectKnowledge
// Created: AGE 2005-03-25
// -----------------------------------------------------------------------------
void DEC_Agent_PathfinderRule::AddObjectKnowledge( const DEC_Path_KnowledgeObject& object, MT_Float rCost )
{
    objects_.push_back( object );
    objects_.back().SetCost( rCost );
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathfinderRule::SetMaximumEnemyCost
// Created: AGE 2005-08-04
// -----------------------------------------------------------------------------
void DEC_Agent_PathfinderRule::SetMaximumEnemyCost( MT_Float rNewCost )
{
    rMaxEnemyCost_ = rNewCost;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathfinderRule::SetDangerDirection
// Created: AGE 2005-08-12
// -----------------------------------------------------------------------------
void DEC_Agent_PathfinderRule::SetDangerDirection( const MT_Vector2D& dangerDirection, MT_Float rBaseCost, MT_Float rLinearCost )
{
    dangerDirection_            = dangerDirection;
    rDangerDirectionBaseCost_   = rBaseCost;
    rDangerDirectionLinearCost_ = rLinearCost;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathfinderRule::SetPreferedTerrain
// Created: AGE 2005-08-04
// -----------------------------------------------------------------------------
void DEC_Agent_PathfinderRule::SetPreferedTerrain( const TerrainData& data, MT_Float rCostOut )
{
    prefer_      = data;
    if( data == TerrainData() )
        rCostOut = 0;
    rPreferCost_ = rCostOut;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathfinderRule::SetAvoidedTerrain
// Created: AGE 2005-08-04
// -----------------------------------------------------------------------------
void DEC_Agent_PathfinderRule::SetAvoidedTerrain( const TerrainData& data, MT_Float rCostIn )
{
    avoid_      = data;
    if( data == TerrainData() )
        rCostIn = 0;
    rAvoidCost_ = rCostIn;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathfinderRule::SetAltitudePreference
// Created: AGE 2005-08-04
// -----------------------------------------------------------------------------
void DEC_Agent_PathfinderRule::SetAltitudePreference( MT_Float rMaxSlope, MT_Float rCostPerMeter )
{
    rMaxSlope_             = rMaxSlope;
    rAltitudeCostPerMeter_ = rCostPerMeter;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathfinderRule::SetFuseau
// Created: AGE 2005-08-12
// -----------------------------------------------------------------------------
void DEC_Agent_PathfinderRule::SetFuseau( const MIL_Fuseau& fuseau, MT_Float rMaxDistanceOut, MT_Float rCostPerMeterOut, MT_Float rComfortDistanceIn, MT_Float rCostPerMeterIn )
{
    pFuseau_                = &fuseau;
    rMaximumFuseauDistance_ = rMaxDistanceOut;
    rComfortFuseauDistance_ = rComfortDistanceIn;
    rFuseauCostPerMeterOut_ = rCostPerMeterOut; 
    rFuseauCostPerMeterIn_  = rCostPerMeterIn;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathfinderRule::SetAutomataFuseau
// Created: AGE 2005-08-12
// -----------------------------------------------------------------------------
void DEC_Agent_PathfinderRule::SetAutomataFuseau( const MIL_Fuseau& fuseau, MT_Float rMaxDistanceOut, MT_Float rCostPerMeterOut )
{
    pAutomateFuseau_                = &fuseau;
    rMaximumAutomataFuseauDistance_ = rMaxDistanceOut;
    rAutomataFuseauCostPerMeterOut_ = rCostPerMeterOut; 
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

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathfinderRule::GetCost
// Created: AGE 2005-03-08
// -----------------------------------------------------------------------------
MT_Float DEC_Agent_PathfinderRule::GetCost( const MT_Vector2D& from, const MT_Vector2D& to, const TerrainData& nToTerrainType, const TerrainData& nLinkTerrainType ) const
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
    {
        const MT_Float rFuseauCost = pFuseau_ ? pFuseau_->GetCost( from, to, rMaximumFuseauDistance_, rFuseauCostPerMeterOut_, rComfortFuseauDistance_, rFuseauCostPerMeterIn_ ) : 0;
        if( rFuseauCost < 0 )
            return rFuseauCost;
        rDynamicCost += rFuseauCost;
    }
    
    // $$$$ AGE 2005-06-24: Going out of the automate fuseau is a no-no. Avoid precision issues
    {
        const MT_Float rAutomateFuseauCost = pAutomateFuseau_ ? pAutomateFuseau_->GetCost( from, to, rMaximumAutomataFuseauDistance_, rAutomataFuseauCostPerMeterOut_, 0, 0 ) : 0;
        if( rAutomateFuseauCost < 0 )
            return rAutomateFuseauCost;
        rDynamicCost += rAutomateFuseauCost;
    }
    
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
        for( DEC_Agent_Path::CIT_PathKnowledgeObjectVector itKnowledge = objects_.begin(); itKnowledge != objects_.end(); ++itKnowledge )
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
        for( DEC_Agent_Path::CIT_PathKnowledgeAgentVector itKnowledge = agents_.begin(); itKnowledge != agents_.end(); ++itKnowledge )
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
// Name: DEC_Agent_PathfinderRule::TerrainCost
// Created: AGE 2005-03-25
// -----------------------------------------------------------------------------
MT_Float DEC_Agent_PathfinderRule::TerrainCost( const TerrainData& data ) const
{
    MT_Float rDynamicCost = 0;
    if( rAvoidCost_ > 0 && avoid_.ContainsOne( data ) )
        rDynamicCost += rAvoidCost_;

    if( rPreferCost_ > 0 && ! prefer_.ContainsOne( data ) )
        rDynamicCost += rPreferCost_;
    return rDynamicCost;
}

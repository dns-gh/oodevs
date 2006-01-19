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
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Path/DEC_Population_PathfinderRule.cpp $
// $Author: Nld $
// $Modtime: 20/07/05 18:46 $
// $Revision: 10 $
// $Workfile: DEC_Population_PathfinderRule.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"
#include "DEC_Population_PathfinderRule.h"
#include "DEC_Population_Path.h"
#include "Decision/Path/DEC_PathType.h"

// -----------------------------------------------------------------------------
// Name: DEC_Population_PathfinderRule constructor
// Created: AGE 2005-03-23
// -----------------------------------------------------------------------------
DEC_Population_PathfinderRule::DEC_Population_PathfinderRule()
    : TerrainRule_ABC()
{
}

// -----------------------------------------------------------------------------
// Name: DEC_Population_PathfinderRule destructor
// Created: AGE 2005-03-08
// -----------------------------------------------------------------------------
DEC_Population_PathfinderRule::~DEC_Population_PathfinderRule()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Population_PathfinderRule::AddChanneler
// Created: SBO 2006-01-16
// -----------------------------------------------------------------------------
void DEC_Population_PathfinderRule::AddChanneler( const DEC_Population_Path_Channeler& channeler )
{
    channelers_.push_back( channeler );
}

// -----------------------------------------------------------------------------
// Name: DEC_Population_PathfinderRule::EvaluateCost
// Created: AGE 2005-03-08
// -----------------------------------------------------------------------------
float DEC_Population_PathfinderRule::EvaluateCost( const geometry::Point2f& from, const geometry::Point2f& to )
{
    return from.Distance( to );
}

// -----------------------------------------------------------------------------
// Name: DEC_Population_PathfinderRule::GetCost
// Created: AGE 2005-03-08
// -----------------------------------------------------------------------------
float DEC_Population_PathfinderRule::GetCost( const geometry::Point2f& from, const geometry::Point2f& to, const TerrainData& terrainTo, const TerrainData& terrainBetween )
{
    static const TerrainData preferedTerrain( TerrainData::SmallRoad  ()
                                      .Merge( TerrainData::MediumRoad () )
                                      .Merge( TerrainData::LargeRoad  () )
                                      .Merge( TerrainData::Urban      () )
                                      .Merge( TerrainData::UrbanBorder() ) );

    const MT_Float rTerrainCost = terrainBetween.ContainsOne( preferedTerrain ) ? 0 : 10000.;

    MT_Vector2D vFrom( from.X(), from.Y() ); 
    MT_Vector2D vTo  ( to.X()  , to.Y() );
    MT_Float rChannelingCost = 0.;
    for( CIT_PathChannelers it = channelers_.begin(); it != channelers_.end(); ++it )
        rChannelingCost += it->ComputeCost( vFrom, vTo, terrainTo, terrainBetween );
    
    return from.Distance( to ) * ( 1 + rChannelingCost + rTerrainCost ) ;
}

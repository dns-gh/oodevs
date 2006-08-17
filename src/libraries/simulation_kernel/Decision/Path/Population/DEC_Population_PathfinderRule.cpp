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

#include "simulation_kernel_pch.h"

#include "DEC_Population_PathfinderRule.h"

#include "DEC_Population_Path.h"

// -----------------------------------------------------------------------------
// Name: DEC_Population_PathfinderRule constructor
// Created: AGE 2005-03-23
// -----------------------------------------------------------------------------
DEC_Population_PathfinderRule::DEC_Population_PathfinderRule( const DEC_Population_Path& path ) 
    : TerrainRule_ABC()
{
    const DEC_Population_Path::T_PopulationPathChannelerVector& channelers = path.GetChannelers();
    channelers_.reserve( channelers.size() );
    for( DEC_Population_Path::CIT_PopulationPathChannelerVector it = channelers.begin(); it != channelers.end(); ++it )
        channelers_.push_back( *it );
}

// -----------------------------------------------------------------------------
// Name: DEC_Population_PathfinderRule destructor
// Created: AGE 2005-03-08
// -----------------------------------------------------------------------------
DEC_Population_PathfinderRule::~DEC_Population_PathfinderRule()
{
    //NOTHING
}

// =============================================================================
// PATH FIND
// =============================================================================

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
                                      .Merge( TerrainData::UrbanBorder() ) );

    const double rTerrainCost = terrainBetween.ContainsOne( preferedTerrain ) ? 0 : 10000.;

    MT_Vector2D vFrom( from.X(), from.Y() ); 
    MT_Vector2D vTo  ( to.X()  , to.Y() );
    double rChannelingCost = 0.;
    for( CIT_PathChannelers it = channelers_.begin(); it != channelers_.end(); ++it )
        rChannelingCost += it->ComputeCost( vFrom, vTo, terrainTo, terrainBetween );
    
    return float( from.Distance( to ) * ( 1 + rChannelingCost + rTerrainCost ) );
}

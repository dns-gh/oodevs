// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Population_PathfinderRule.h"
#include "DEC_Population_Path.h"
#include "simulation_terrain/TER_World.h"
#include <spatialcontainer/TerrainData.h>

// -----------------------------------------------------------------------------
// Name: DEC_Population_PathfinderRule constructor
// Created: AGE 2005-03-23
// -----------------------------------------------------------------------------
DEC_Population_PathfinderRule::DEC_Population_PathfinderRule( const DEC_Population_Path& path )
    : TerrainRule_ABC()
    , path_          ( path )
    , world_         ( TER_World::GetWorld() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Population_PathfinderRule destructor
// Created: AGE 2005-03-08
// -----------------------------------------------------------------------------
DEC_Population_PathfinderRule::~DEC_Population_PathfinderRule()
{
    // NOTHING
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
// Name: DEC_Population_PathfinderRule::GetChannelingCost
// Created: NLD 2007-03-07
// -----------------------------------------------------------------------------
float DEC_Population_PathfinderRule::GetChannelingCost( const MT_Vector2D& vFrom, const MT_Vector2D& vTo, const TerrainData& terrainTo, const TerrainData& terrainBetween ) const
{
    const DEC_Population_Path::T_PopulationPathChannelerVector& channelers = path_.GetChannelers();
    if( channelers.empty() )
        return 0.f;
    for( DEC_Population_Path::CIT_PopulationPathChannelerVector it = channelers.begin(); it != channelers.end(); ++it )
        if( it->ComputeCost( vFrom, vTo, terrainTo, terrainBetween ) != std::numeric_limits< double >::min() ) // Inside channel
            return 0.f;
    return static_cast< float >( path_.GetCostOutsideOfChanneling() );
}

// -----------------------------------------------------------------------------
// Name: DEC_Population_PathfinderRule::GetTerrainCost
// Created: CMA 2011-05-20
// -----------------------------------------------------------------------------
float DEC_Population_PathfinderRule::GetTerrainCost( const TerrainData& nToTerrainType, const TerrainData& nLinkTerrainType ) const
{
    static const unsigned short urbanZone = TerrainData::Urban().Area();
    static const unsigned short preferedBorders = TerrainData::ForestBorder().Border() | TerrainData::PlantationBorder().Border();
    static const unsigned short difficultZone = TerrainData::Forest().Area() | TerrainData::Plantation().Area() | TerrainData::Dune().Area() | TerrainData::Ice().Area() | TerrainData::Mountain().Area();

    if( nLinkTerrainType.IsRoad() )
        return 1.f;
    else if( nToTerrainType.Area() & difficultZone )
        return 10.f;
    else if( ( nToTerrainType.Border() & preferedBorders ) || ( ( nToTerrainType.Area() & urbanZone ) && !( nToTerrainType.Area() & difficultZone ) ) )
        return 2.5f;
    else
        return 5.f;
}

// -----------------------------------------------------------------------------
// Name: DEC_Population_PathfinderRule::GetCost
// Created: AGE 2005-03-08
// -----------------------------------------------------------------------------
float DEC_Population_PathfinderRule::GetCost( const geometry::Point2f& from, const geometry::Point2f& to, const TerrainData& terrainTo, const TerrainData& terrainBetween )
{
    static const unsigned short river = TerrainData::SmallRiver().Linear() | TerrainData::MediumRiver().Linear() | TerrainData::LargeRiver().Linear();
    static const unsigned short waterZone = TerrainData::Water().Area() | TerrainData::Swamp().Area();
    static const unsigned short cliff = TerrainData::Cliff().Linear();

    MT_Vector2D vFrom( from.X(), from.Y() );
    MT_Vector2D vTo( to.X(), to.Y() );

    if( !world_.IsValidPosition( vTo ) )
        return -1.f;

    if( ( ( terrainTo.Linear() & river ) || ( terrainTo.Area() & waterZone ) || ( terrainTo.Linear() & cliff ) ) && !terrainBetween.IsRoad() )
        return -1.f;

    float rDynamicCost = 0.;

    const float rTerrainCost = GetTerrainCost( terrainTo, terrainBetween );
    rDynamicCost += rTerrainCost;

    const float rChannelingCost = GetChannelingCost( vFrom, vTo, terrainTo, terrainBetween );
    rDynamicCost += rChannelingCost;

    const float rDistance = from.Distance( to );
    return ( rDistance * rDynamicCost );
}

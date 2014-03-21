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
#include "DEC_Path_KnowledgeObject_ABC.h"
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
    const auto& channelers = path_.GetChannelers();
    if( channelers.empty() )
        return 0.f;
    for( auto it = channelers.begin(); it != channelers.end(); ++it )
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
// Name: DEC_Population_PathfinderRule::GetObjectsCost
// Created: CMA 2011-11-24
// -----------------------------------------------------------------------------
float DEC_Population_PathfinderRule::GetObjectsCost( const MT_Vector2D& from, const MT_Vector2D& to, const TerrainData& nToTerrainType, const TerrainData& nLinkTerrainType ) const
{
    // default cost : outside all objects
    double rObjectCost = path_.GetCostOutsideOfAllObjects();
    const auto& knowledgesByTypes = path_.GetPathKnowledgeObjects();
    for( auto itType = knowledgesByTypes.begin(); itType != knowledgesByTypes.end(); ++itType )
    {
        bool bInsideObjectType = false;
        const auto& knowledges = *itType;
        for( auto itKnowledge = knowledges.begin(); itKnowledge != knowledges.end(); ++itKnowledge )
        {
            double rCurrentObjectCost = ( *itKnowledge )->ComputeCost( from, to, nToTerrainType, nLinkTerrainType, 0. );
            if( rCurrentObjectCost != std::numeric_limits< double >::min()  )
            {
                if( !bInsideObjectType )
                {
                    rObjectCost -= ( *itKnowledge )->GetCostOut();
                    bInsideObjectType = true;
                }
                if( rCurrentObjectCost < 0. ) // Impossible move (for example destroyed bridge)
                    return (float) rCurrentObjectCost;
                rObjectCost += rCurrentObjectCost;
            }
        }
    }
    return (float) rObjectCost;
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

    const double rSpeed = path_.GetMaxSpeed( terrainBetween );
    if( rSpeed <= 0 )
        return -1.f;

    MT_Vector2D vFrom( from.X(), from.Y() );
    MT_Vector2D vTo( to.X(), to.Y() );

    if( !world_.IsValidPosition( vTo ) )
        return -1.f;

    if( ( ( terrainTo.Linear() & river ) || ( terrainTo.Area() & waterZone ) || ( terrainTo.Linear() & cliff ) ) && !terrainBetween.IsRoad() )
        return -1.f;

    // Cost computation taken various dynamic terrain items into account
    float rDynamicCost = 0.;

    // terrain type
    const float rTerrainCost = GetTerrainCost( terrainTo, terrainBetween );
    if( rTerrainCost < 0 )
        return -1.f;
    rDynamicCost += rTerrainCost;

    // population channel
    const float rChannelingCost = GetChannelingCost( vFrom, vTo, terrainTo, terrainBetween );
    rDynamicCost += rChannelingCost;

    // objects
    const float rObjectsCost = GetObjectsCost( vFrom, vTo, terrainTo, terrainBetween );
    if( rObjectsCost < 0 )
        return -1.f;
    rDynamicCost += rObjectsCost;
    const float rDistance = from.Distance( to );
    const double maxSpeed = path_.GetMaxSpeed();
    return static_cast< float >( rDistance * rDynamicCost * ( maxSpeed ? maxSpeed / rSpeed : 1 ) );
}

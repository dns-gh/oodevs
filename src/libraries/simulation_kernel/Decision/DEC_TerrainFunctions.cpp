// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_TerrainFunctions.h"
#include "DEC_Decision.h"
#include "Brain.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Terrain/PHY_RoleInterface_TerrainAnalysis.h"
#include "simulation_terrain/TER_Analyzer.h"
#include "simulation_terrain/TER_Localisation.h"
#include "simulation_terrain/TER_World.h"
#include <spatialcontainer/TerrainData.h>

// -----------------------------------------------------------------------------
// Name: DEC_TerrainFunctions::GetCrossroads
// Created: MGD 2009-08-19
// -----------------------------------------------------------------------------
void DEC_TerrainFunctions::GetCrossroads( sword::Brain& brain, MIL_AgentPion& pion, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& table )
{
    std::vector< boost::shared_ptr< MT_Vector2D > > points;
    pion.GetRole< PHY_RoleInterface_TerrainAnalysis >().GetCrossroads( points );
    knowledgeCreateFunction( table, brain.GetScriptRef( "integration.ontology.types.point" ), points, true );
}

// -----------------------------------------------------------------------------
// Name: DEC_TerrainFunctions::FindSafetyPositionsWithinCircle
// Created: LDC 2010-10-27
// -----------------------------------------------------------------------------
std::vector< boost::shared_ptr< MT_Vector2D > > DEC_TerrainFunctions::FindSafetyPositionsWithinCircle( MIL_AgentPion& pion, float radius, float safetyDistance )
{
    std::vector< boost::shared_ptr< MT_Vector2D > > points;
    pion.GetRole< PHY_RoleInterface_TerrainAnalysis >().FindSafetyPositionsWithinCircle( points, radius, safetyDistance );
    return points;
}

// -----------------------------------------------------------------------------
// Name: DEC_TerrainFunctions::GetRoadIntersectionsWithZone
// Created: LDC 2011-06-22
// -----------------------------------------------------------------------------
std::vector< boost::shared_ptr< MT_Vector2D > > DEC_TerrainFunctions::GetRoadIntersectionsWithZone( const TER_Localisation* zone )
{
    if( !zone )
        throw MASA_EXCEPTION( "invalid parameter." );
    std::vector< boost::shared_ptr< MT_Vector2D > > points;
    if( zone->GetType() == TER_Localisation::ePolygon )
    {
        TER_Polygon polygon;
        polygon.Reset( zone->GetPoints() );
        TER_World::GetWorld().GetAnalyzer().FindRoadsOnBorderOfPolygon( polygon, points );
    }
    else
        MT_LOG_INFO_MSG( "TER_Localisation parameter of DEC_TerrainFunctions::GetRoadIntersectionsWithZone method should be a polygon" );
    return points;
}

// -----------------------------------------------------------------------------
// Name: DEC_TerrainFunctions::CanMoveOn
// Created: CMA 2011-08-30
// -----------------------------------------------------------------------------
bool DEC_TerrainFunctions::CanMoveOn( const DEC_Decision_ABC* agent, boost::shared_ptr< MT_Vector2D > position )
{
    if( !agent )
        throw MASA_EXCEPTION( "invalid parameter." );
    if( !position )
        return false;
    return agent->GetPion().GetRole< PHY_RoleInterface_TerrainAnalysis >().CanMoveOn( *position );
}

// -----------------------------------------------------------------------------
// Name: DEC_TerrainFunctions::IsLinearRiverInBetween
// Created: LDC 2013-10-29
// -----------------------------------------------------------------------------
bool DEC_TerrainFunctions::IsLinearRiverInBetween( const MT_Vector2D* from, const MT_Vector2D* to )
{
    if( !from || !to )
        throw MASA_EXCEPTION( "Invalid point in call to ArePointsOnSameRiverBank" );
    std::vector< boost::shared_ptr< MT_Vector2D > > positions;
    TerrainData rivers;
    rivers.Merge( TerrainData::SmallRiver() ).Merge( TerrainData::MediumRiver() ).Merge( TerrainData::LargeRiver() );
    TER_Analyzer::GetAnalyzer().FindSegmentIntersections( *from, *to, rivers, positions );
    return !positions.empty();
}

// -----------------------------------------------------------------------------
// Name: DEC_TerrainFunctions::DEC_TerrainFunctions::IsWaterInBetween
// Created: LDC 2013-10-31
// -----------------------------------------------------------------------------
bool DEC_TerrainFunctions::IsWaterInBetween( const MT_Vector2D* from, const MT_Vector2D* to )
{
    if( !from || !to )
        throw MASA_EXCEPTION( "Invalid point in call to ArePointsOnSameRiverBank" );
    std::vector< boost::shared_ptr< MT_Vector2D > > positions;
    TerrainData rivers;
    TER_Analyzer::GetAnalyzer().FindSegmentIntersections( *from, *to, TerrainData::WaterBorder(), positions );
    return !positions.empty();
}

// -----------------------------------------------------------------------------
// Name: DEC_TerrainFunctions::DEC_TerrainFunctions::GetTerrainData
// Created: SLI 2014-04-03
// -----------------------------------------------------------------------------
std::pair< unsigned int, unsigned int > DEC_TerrainFunctions::GetTerrainData( const MT_Vector2D* location )
{
    const TerrainData data = TER_Analyzer::GetAnalyzer().Pick( *location );
    return std::make_pair( data.Area(), data.Linear() );
}

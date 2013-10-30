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
#include "simulation_terrain/TER_World.h"
#include "simulation_terrain/TER_AnalyzerManager.h"

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
        TER_World::GetWorld().GetAnalyzerManager().FindRoadsOnBorderOfPolygon( polygon, points );
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
// Name: DEC_TerrainFunctions::ArePointsOnSameRiverBank
// Created: LDC 2013-10-29
// This is a very raw algorithm. It handles a river loop but not tributaries/junctions/bridges...
// A more exact solution would require to run a pathfind to check if destination is reachable
// without crossing a river, but I don't want to have to wait for a pathfind result,
// particularly because the 'to' point is likely not reachable, so it would take forever to compute.
// -----------------------------------------------------------------------------
bool DEC_TerrainFunctions::ArePointsOnSameRiverBank( const MT_Vector2D* from, const MT_Vector2D* to )
{
    if( !from || !to )
        throw MASA_EXCEPTION( "Invalid point in call to IsRiverSegment" );
    std::vector< boost::shared_ptr< MT_Vector2D > > positions;
    TER_AnalyzerManager::GetAnalyzerManager().FindRiverSegmentIntersections( *from, *to, positions );
    if ( positions.size() % 2 == 1 )
        return false;
    // Surfacic...
    std::vector< MT_Vector2D > line;
    line.push_back( *from );
    line.push_back( *to );
    TER_Localisation location( TER_Localisation::eLine, line );
    TerrainData data = TER_AnalyzerManager::GetAnalyzerManager().GetTerrainData( location );
    return !data.ContainsOne( TerrainData::Water() );
}

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_terrain_pch.h"
#include "TER_AnalyzerManager.h"
#include "TER_Analyzer.h"
#include "TER_World.h"
#include <spatialcontainer/TerrainData.h>

// -----------------------------------------------------------------------------
// Name: TER_AnalyzerManager::GetAnalyzerManager
// Created: CMA 2011-08-16
// -----------------------------------------------------------------------------
TER_AnalyzerManager& TER_AnalyzerManager::GetAnalyzerManager()
{
    return TER_World::GetWorld().GetAnalyzerManager();
}

// -----------------------------------------------------------------------------
// Name: TER_AnalyzerManager constructor
// Created: CMA 2011-08-16
// -----------------------------------------------------------------------------
TER_AnalyzerManager::TER_AnalyzerManager( const TER_StaticData& staticData )
    : staticData_( staticData )
{
    pAnalyzer_ = new TER_Analyzer( staticData_ );
}

// -----------------------------------------------------------------------------
// Name: TER_AnalyzerManager destructor
// Created: CMA 2011-08-16
// -----------------------------------------------------------------------------
TER_AnalyzerManager::~TER_AnalyzerManager()
{
    delete pAnalyzer_;
}

// -----------------------------------------------------------------------------
// Name: TER_AnalyzerManager::FindCrossroadsWithinCircle
// Created: CMA 2011-08-16
// -----------------------------------------------------------------------------
std::vector< boost::shared_ptr< MT_Vector2D > > TER_AnalyzerManager::FindCrossroadsWithinCircle( const MT_Vector2D& center, float radius )
{
    return pAnalyzer_->FindCrossroadsWithinCircle( center, radius );
}

// -----------------------------------------------------------------------------
// Name: TER_AnalyzerManager::FindSafetyPositionsWithinCircle
// Created: CMA 2011-08-16
// -----------------------------------------------------------------------------
void TER_AnalyzerManager::FindSafetyPositionsWithinCircle( const MT_Vector2D& center, float radius, float safetyDistance, std::vector< boost::shared_ptr< MT_Vector2D > >& positions )
{
    pAnalyzer_->FindSafetyPositionsWithinCircle( center, radius, safetyDistance, positions );
}

// -----------------------------------------------------------------------------
// Name: TER_AnalyzerManager::FindRoadsOnBorderOfPolygon
// Created: CMA 2011-09-01
// -----------------------------------------------------------------------------
void TER_AnalyzerManager::FindRoadsOnBorderOfPolygon( const TER_Polygon& polygon, std::vector< boost::shared_ptr< MT_Vector2D > >& positions )
{
    pAnalyzer_->FindRoadsOnBorderOfPolygon( polygon, positions );
}

// -----------------------------------------------------------------------------
// Name: TER_AnalyzerManager::FindSegmentIntersections
// Created: LDC 2013-10-30
// -----------------------------------------------------------------------------
void TER_AnalyzerManager::FindSegmentIntersections( const MT_Vector2D& from, const MT_Vector2D& to, const TerrainData& terrainSought, std::vector< boost::shared_ptr< MT_Vector2D > >& positions )
{
    pAnalyzer_->FindSegmentIntersections( from, to, terrainSought, positions );
}

// -----------------------------------------------------------------------------
// Name: TER_AnalyzerManager::FindAllPositions
// Created: CMA 2011-08-16
// -----------------------------------------------------------------------------
std::vector< boost::shared_ptr< MT_Vector2D > > TER_AnalyzerManager::FindAllPositions( const MT_Vector2D& center, float radius )
{
    return pAnalyzer_->FindAllPositions( center, radius );
}

// -----------------------------------------------------------------------------
// Name: TER_AnalyzerManager::FindTerrainDataWithinCircle
// Created: CMA 2011-08-16
// -----------------------------------------------------------------------------
TerrainData TER_AnalyzerManager::FindTerrainDataWithinCircle( const MT_Vector2D& center, float radius )
{
    return pAnalyzer_->FindTerrainDataWithinCircle( center, radius );
}

// -----------------------------------------------------------------------------
// Name: TER_AnalyzerManager::FindTerrainDataWithinPolygon
// Created: JSR 2012-02-06
// -----------------------------------------------------------------------------
TerrainData TER_AnalyzerManager::FindTerrainDataWithinPolygon( const TER_Polygon& polygon )
{
    return pAnalyzer_->FindTerrainDataWithinPolygon( polygon );
}

// -----------------------------------------------------------------------------
// Name: TER_AnalyzerManager::Pick
// Created: CMA 2011-08-29
// -----------------------------------------------------------------------------
TerrainData TER_AnalyzerManager::Pick( const MT_Vector2D& pos )
{
    return pAnalyzer_->Pick( pos );
}

// -----------------------------------------------------------------------------
// Name: TER_AnalyzerManager::GetTerrainData
// Created: LDC 2013-05-06
// -----------------------------------------------------------------------------
TerrainData TER_AnalyzerManager::GetTerrainData( const TER_Localisation& localisation )
{
    return pAnalyzer_->GetTerrainData( localisation );
}

void TER_AnalyzerManager::ApplyOnNodesWithinCircle( const MT_Vector2D& vCenter, double rRadius, T_Functor& bestNodeFunction ) const
{
    pAnalyzer_->ApplyOnNodesWithinCircle( vCenter, rRadius, bestNodeFunction );
}

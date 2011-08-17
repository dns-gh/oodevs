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
// Name: TER_AnalyzerManager::DefaultTerrainData
// Created: AGE 2005-02-01
// -----------------------------------------------------------------------------
TerrainData& TER_AnalyzerManager::DefaultTerrainData()
{
    static TerrainData data;
    return data;
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
std::vector< boost::shared_ptr< MT_Vector2D > > TER_AnalyzerManager::FindSafetyPositionsWithinCircle( const MT_Vector2D& center, float radius, float safetyDistance )
{
    return pAnalyzer_->FindSafetyPositionsWithinCircle( center, radius, safetyDistance );
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

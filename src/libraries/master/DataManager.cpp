// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "master_pch.h"
#include "DataManager.h"

#include "Config.h"
#include "Exercise.h"

#pragma warning( push )
#pragma warning( disable: 4127 4512 4511 )
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#pragma warning( pop )

namespace bfs = boost::filesystem;

using namespace master;

#include <iostream>

// -----------------------------------------------------------------------------
// Name: DataManager constructor
// Created: NLD 2007-01-29
// -----------------------------------------------------------------------------
DataManager::DataManager( const Master& master, const Config& config )
    : master_   ( master )
    , terrains_ ()
    , datasets_ ()
    , exercises_()
{
    InitializeTerrains ( config );
    InitializeDatasets ( config );
    InitializeExercises( config );
}

// -----------------------------------------------------------------------------
// Name: DataManager destructor
// Created: NLD 2007-01-29
// -----------------------------------------------------------------------------
DataManager::~DataManager()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DataManager::InitializeTerrains
// Created: NLD 2007-01-29
// -----------------------------------------------------------------------------
void DataManager::InitializeTerrains( const Config& config )
{
    const bfs::path terrainsDir( config.GetTerrainsDir(), bfs::native );

    for( bfs::directory_iterator it( terrainsDir ); it != bfs::directory_iterator(); ++it )
    {
        bfs::path current = *it;
        const std::string leaf( current.leaf() );
        if( bfs::is_directory( current ) && bfs::exists( bfs::path( config.GetTerrainFile( leaf ), bfs::native ) ) )
            terrains_.Create( leaf, *this, config, leaf );
    }
}

// -----------------------------------------------------------------------------
// Name: DataManager::InitializeDatasets
// Created: NLD 2007-01-29
// -----------------------------------------------------------------------------
void DataManager::InitializeDatasets( const Config& config )
{
    const bfs::path modelsDir( config.GetModelsDir(), bfs::native );

    for( bfs::directory_iterator it( modelsDir ); it != bfs::directory_iterator(); ++it )
    {
        bfs::path current = *it;
        const std::string leaf( current.leaf() );
        if( bfs::is_directory( current ) && bfs::exists( bfs::path( config.GetDecisionalFile( leaf ), bfs::native ) ) )
            datasets_.Create( leaf, *this, config, leaf );
    }
}

// -----------------------------------------------------------------------------
// Name: DataManager::InitializeExercises
// Created: NLD 2007-01-29
// -----------------------------------------------------------------------------
void DataManager::InitializeExercises( const Config& config )
{
    const bfs::path exercisesDir( config.GetModelsDir(), bfs::native );

    for( bfs::directory_iterator it( exercisesDir ); it != bfs::directory_iterator(); ++it )
    {
        bfs::path current = *it;
        const std::string leaf( current.leaf() );
        if( bfs::is_directory( current ) && bfs::exists( bfs::path( config.GetExerciseFile( leaf ), bfs::native ) ) )
            exercises_.Create( leaf, *this, config, leaf );
    }
    
    if( bfs::exists( bfs::path( config.GetExerciseFile( "" ), bfs::native ) ) )
        datasets_.Create( "", *this, config, "" );
}

// =============================================================================
// ACCESSORS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ModelsContainer< std::string, Terrain  >& DataManager::GetTerrains
// Created: NLD 2007-01-29
// -----------------------------------------------------------------------------
const ModelsContainer< std::string, Terrain  >& DataManager::GetTerrains() const
{
    return terrains_;
}

// -----------------------------------------------------------------------------
// Name: ModelsContainer< std::string, Dataset  >& DataManager::GetDatasets
// Created: NLD 2007-01-29
// -----------------------------------------------------------------------------
const ModelsContainer< std::string, Dataset  >& DataManager::GetDatasets() const
{
    return datasets_;
}

// -----------------------------------------------------------------------------
// Name: ModelsContainer< std::string, Exercise >& DataManager::GetExercises
// Created: NLD 2007-01-29
// -----------------------------------------------------------------------------
const ModelsContainer< std::string, Exercise >& DataManager::GetExercises() const
{
    return exercises_;
}

// =============================================================================
// OEPRATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DataManager::Send
// Created: NLD 2007-01-31
// -----------------------------------------------------------------------------
void DataManager::Send( Publisher_ABC& publisher ) const
{
    terrains_ .Apply( std::mem_fun_ref( &Terrain ::SendCreation ), publisher );
    datasets_ .Apply( std::mem_fun_ref( &Dataset ::SendCreation ), publisher );
    exercises_.Apply( std::mem_fun_ref( &Exercise::SendCreation ), publisher );
}

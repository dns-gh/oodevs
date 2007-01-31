// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "master_pch.h"
#include "Exercise.h"

#include "Config.h"
#include "DataManager.h"
#include "Master.h"
#include "xeumeuleu/xml.h"

using namespace master;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: Exercise constructor
// Created: NLD 2007-01-29
// -----------------------------------------------------------------------------
Exercise::Exercise( const DataManager& dataManager, const Config& config, const std::string& name )
    : name_          ( name )
    , pTerrain_      ( 0 )
    , pDataset_      ( 0 )
    , pPhysicalModel_( 0 )
{
    xifstream xis( config.GetExerciseFile( name ) );

    std::string terrain, dataset, physical, weather, orbat;
    xis >> start( "exercise" )
            >> start( "terrain" )
                >> attribute( "name", terrain )
            >> end()
            >> start( "model" )
                >> attribute( "dataset", dataset )
                >> attribute( "physical", physical )
            >> end()
            >> start( "weather" )
                >> attribute( "file", weather )
            >> end()
            >> start( "orbat" )
                >> attribute( "file", orbat )
            >> end();

    pTerrain_ = dataManager.GetTerrains().Find( terrain );
    if( !pTerrain_ )
        throw std::runtime_error( "Invalid terrain for exercise '" + name + "'" );

    pDataset_ = dataManager.GetDatasets().Find( dataset );
    if( !pDataset_ )
        throw std::runtime_error( "Invalid dataset for exercise '" + name + "'" );

    pPhysicalModel_ = pDataset_->GetPhysicalModels().Find( physical );
    if( !pPhysicalModel_ )
        throw std::runtime_error( "Invalid physical model for exercise '" + name + "'" );  

    MT_LOG_INFO_MSG( "Exercise loaded : '" << name_ << "'" );
}
    
// -----------------------------------------------------------------------------
// Name: Exercise destructor
// Created: NLD 2007-01-29
// -----------------------------------------------------------------------------
Exercise::~Exercise()
{
    // NOTHING
}

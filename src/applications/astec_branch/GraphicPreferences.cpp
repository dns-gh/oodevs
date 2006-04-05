// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "GraphicPreferences.h"
#include "TerrainPreference.h"

// -----------------------------------------------------------------------------
// Name: GraphicPreferences constructor
// Created: SBO 2006-04-04
// -----------------------------------------------------------------------------
GraphicPreferences::GraphicPreferences()
{
    InitializeTerrainPreferences();
}

// -----------------------------------------------------------------------------
// Name: GraphicPreferences destructor
// Created: SBO 2006-04-04
// -----------------------------------------------------------------------------
GraphicPreferences::~GraphicPreferences()
{
    for( IT_TerrainPreferences it = terrainPrefs_.begin(); it != terrainPrefs_.end(); ++it )
        delete (*it).second;
}

// -----------------------------------------------------------------------------
// Name: GraphicPreferences::ReadPreferences
// Created: SBO 2006-04-04
// -----------------------------------------------------------------------------
void GraphicPreferences::ReadPreferences()
{
    for( CIT_TerrainPreferences it = terrainPrefs_.begin(); it != terrainPrefs_.end(); ++it )
        (*it).second->Read();
}
    
// -----------------------------------------------------------------------------
// Name: GraphicPreferences::WritePreferences
// Created: SBO 2006-04-04
// -----------------------------------------------------------------------------
void GraphicPreferences::WritePreferences()
{
    for( CIT_TerrainPreferences it = terrainPrefs_.begin(); it != terrainPrefs_.end(); ++it )
        (*it).second->Write();
}

// -----------------------------------------------------------------------------
// Name: GraphicPreferences::InitializeTerrainPreferences
// Created: SBO 2006-04-04
// -----------------------------------------------------------------------------
void GraphicPreferences::InitializeTerrainPreferences()
{
    terrainPrefs_.insert( std::make_pair( TerrainData::Forest()     , new TerrainPreference( "Forest" ) ) );
    terrainPrefs_.insert( std::make_pair( TerrainData::Plantation() , new TerrainPreference( "Plantation" ) ) );
    terrainPrefs_.insert( std::make_pair( TerrainData::Dune()       , new TerrainPreference( "Dune" ) ) );
    terrainPrefs_.insert( std::make_pair( TerrainData::Urban()      , new TerrainPreference( "Urban" ) ) );
    terrainPrefs_.insert( std::make_pair( TerrainData::Water()      , new TerrainPreference( "Water" ) ) );
    terrainPrefs_.insert( std::make_pair( TerrainData::Ice()        , new TerrainPreference( "Ice" ) ) );
    terrainPrefs_.insert( std::make_pair( TerrainData::Cliff()      , new TerrainPreference( "Cliff" ) ) );
    terrainPrefs_.insert( std::make_pair( TerrainData::Motorway()   , new TerrainPreference( "Motorway" ) ) );
    terrainPrefs_.insert( std::make_pair( TerrainData::LargeRoad()  , new TerrainPreference( "LargeRoad" ) ) );
    terrainPrefs_.insert( std::make_pair( TerrainData::MediumRoad() , new TerrainPreference( "MediumRoad" ) ) );
    terrainPrefs_.insert( std::make_pair( TerrainData::SmallRoad()  , new TerrainPreference( "SmallRoad" ) ) );
    terrainPrefs_.insert( std::make_pair( TerrainData::Bridge()     , new TerrainPreference( "Bridge" ) ) );
    terrainPrefs_.insert( std::make_pair( TerrainData::Railroad()   , new TerrainPreference( "Railroad" ) ) );
    terrainPrefs_.insert( std::make_pair( TerrainData::LargeRiver() , new TerrainPreference( "LargeRiver" ) ) );
    terrainPrefs_.insert( std::make_pair( TerrainData::MediumRiver(), new TerrainPreference( "MediumRiver" ) ) );
    terrainPrefs_.insert( std::make_pair( TerrainData::SmallRiver() , new TerrainPreference( "SmallRiver" ) ) );
}

// -----------------------------------------------------------------------------
// Name: GraphicPreferences::AddPanels
// Created: SBO 2006-04-04
// -----------------------------------------------------------------------------
void GraphicPreferences::AddPanels( QTabWidget* parent ) const
{
    AddTerrainPanel( parent );
}

// -----------------------------------------------------------------------------
// Name: GraphicPreferences::AddTerrainPanel
// Created: SBO 2006-04-04
// -----------------------------------------------------------------------------
void GraphicPreferences::AddTerrainPanel( QTabWidget* parent ) const
{
    QVBox* pPanel = new QVBox( parent );
    pPanel->setMargin( 5 );
    parent->addTab( pPanel, "Terrain" );
    for( CIT_TerrainPreferences it = terrainPrefs_.begin(); it != terrainPrefs_.end(); ++it )
        (*it).second->Display( pPanel );
}

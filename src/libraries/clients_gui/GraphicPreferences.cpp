// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "GraphicPreferences.h"
#include "moc_GraphicPreferences.cpp"

#include "TerrainPreference.h"
#include "graphics/GraphicData.h"

#include "xeumeuleu/xml.h"
using namespace xml;

using namespace gui;

// -----------------------------------------------------------------------------
// Name: GraphicPreferences constructor
// Created: SBO 2006-04-04
// -----------------------------------------------------------------------------
GraphicPreferences::GraphicPreferences()
    : alpha_( 1 )
{
    InitializeTerrainPreferences();
}

// -----------------------------------------------------------------------------
// Name: GraphicPreferences destructor
// Created: SBO 2006-04-04
// -----------------------------------------------------------------------------
GraphicPreferences::~GraphicPreferences()
{
    for( CIT_Displays it = displays_.begin(); it != displays_.end(); ++it )
        delete *it;
}

// -----------------------------------------------------------------------------
// Name: GraphicPreferences::InitializeTerrainPreferences
// Created: SBO 2006-04-04
// -----------------------------------------------------------------------------
void GraphicPreferences::InitializeTerrainPreferences()
{
    xml::xifstream xis( "preferences.xml" ); // $$$$ AGE 2006-04-05: 
    xis >> start( "preferences" )
            >> start( "terrains" )
				>> list( "terrain", *this, & GraphicPreferences::ReadTerrainPreference );
}

// -----------------------------------------------------------------------------
// Name: GraphicPreferences::ReadTerrainPreference
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
void GraphicPreferences::ReadTerrainPreference( xml::xistream& xis )
{
    std::string type;
    xis >> attribute( "type", type );
    displays_.push_back( new TerrainPreference( xis ) );
    terrainPrefs_[ TerrainData( type ) ] = displays_.back();
}

// -----------------------------------------------------------------------------
// Name: GraphicPreferences::AddToPanel
// Created: SBO 2006-04-04
// -----------------------------------------------------------------------------
void GraphicPreferences::Display( QWidget* parent ) const
{
    QVBox* pPanel = new QVBox( parent );
    pPanel->setMargin( 5 );

    QGroupBox* alphaBox = new QGroupBox( 1, Qt::Vertical, tr( "Transparency" ), pPanel );
    QSlider* slider = new QSlider( 0, 100, 1, 100, Qt::Horizontal, alphaBox );
    connect( slider, SIGNAL( valueChanged( int ) ), this, SLOT( OnAlpha( int ) ) );
    
    QGroupBox* colorBox = new QGroupBox( displays_.size(), Qt::Vertical, tr( "Colors" ), pPanel );
    for( CIT_Displays it = displays_.begin(); it != displays_.end(); ++it )
        (*it)->Display( colorBox );
}

// -----------------------------------------------------------------------------
// Name: GraphicPreferences::OnAlpha
// Created: AGE 2006-11-22
// -----------------------------------------------------------------------------
void GraphicPreferences::OnAlpha( int value )
{
    alpha_ = value * 0.01f;
}

// -----------------------------------------------------------------------------
// Name: GraphicPreferences::Commit
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
void GraphicPreferences::Commit()
{
     for( CIT_Displays it = displays_.begin(); it != displays_.end(); ++it )
        (*it)->Commit();
     Save();
}

// -----------------------------------------------------------------------------
// Name: GraphicPreferences::Save
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
void GraphicPreferences::Save() const
{
    xml::xofstream xos( "preferences.xml" ); // $$$$ AGE 2006-04-05: 
    xos << start( "preferences" )
            << start( "terrains" );
    for( CIT_Displays it = displays_.begin(); it != displays_.end(); ++it )
        Save( xos, **it );
    xos     << end()
        << end();
}

// -----------------------------------------------------------------------------
// Name: GraphicPreferences::Save
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
void GraphicPreferences::Save( xostream& xos, const TerrainPreference& preference ) const
{
    for( CIT_TerrainPreferences it = terrainPrefs_.begin(); it != terrainPrefs_.end(); ++it )
        if( it->second == &preference )
        {
            xos << start( "terrain" )
                << attribute( "type", it->first.ToString() );
            preference.Save( xos );
            xos << end();
        }
}

// -----------------------------------------------------------------------------
// Name: GraphicPreferences::Revert
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
void GraphicPreferences::Revert()
{
    for( CIT_Displays it = displays_.begin(); it != displays_.end(); ++it )
        (*it)->Revert();
}

// -----------------------------------------------------------------------------
// Name: GraphicPreferences::SetupLineGraphics
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
void GraphicPreferences::SetupLineGraphics( const Data_ABC* pData )
{
    const GraphicData* d = static_cast< const GraphicData* >( pData );
    const TerrainData& data = *d;
    const TerrainPreference* preference = terrainPrefs_[ data ];
    if( preference )
    {
        preference->SetLineWidth();
        preference->SetColor( alpha_ );
    }
}

// -----------------------------------------------------------------------------
// Name: GraphicPreferences::SetupBorderGraphics
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
void GraphicPreferences::SetupBorderGraphics( const Data_ABC* pData )
{
    const GraphicData* d = static_cast< const GraphicData* >( pData );
    const TerrainData& data = *d;
    const TerrainPreference* preference = terrainPrefs_[ data ];
    if( preference )
    {
        preference->SetLineWidth();
        preference->SetColor( alpha_ );
    }
}

// -----------------------------------------------------------------------------
// Name: GraphicPreferences::SetupAreaGraphics
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
void GraphicPreferences::SetupAreaGraphics( const Data_ABC* pData )
{
    const GraphicData* d = static_cast< const GraphicData* >( pData );
    const TerrainData& data = *d;
    const TerrainPreference* preference = terrainPrefs_[ data ];
    if( preference )
        preference->SetColor( alpha_ * 0.5f );
}

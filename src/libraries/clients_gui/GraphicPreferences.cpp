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
#include "TerrainPreference.h"
#include "graphics/GraphicData.h"
#include "clients_kernel/Tools.h"
#include <xeumeuleu/xml.h>
#include <boost/bind.hpp>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: GraphicPreferences constructor
// Created: SBO 2006-04-04
// -----------------------------------------------------------------------------
GraphicPreferences::GraphicPreferences( kernel::Controllers& controllers )
    : controllers_( controllers )
    , alpha_( 1 )
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
    xis >> xml::start( "preferences" )
            >> xml::start( "terrains" )
				>> xml::list( "terrain", *this, & GraphicPreferences::ReadTerrainPreference );
}

// -----------------------------------------------------------------------------
// Name: GraphicPreferences::ReadTerrainPreference
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
void GraphicPreferences::ReadTerrainPreference( xml::xistream& xis )
{
    const std::string type = xml::attribute< std::string >( xis, "type" );
    displays_.push_back( new TerrainPreference( xis, controllers_ ) );
    terrainPrefs_[ TerrainData( type ) ] = displays_.back();
}

// -----------------------------------------------------------------------------
// Name: GraphicPreferences::AddToPanel
// Created: SBO 2006-04-04
// -----------------------------------------------------------------------------
void GraphicPreferences::Display( QWidget* parent ) const
{
    QGroupBox* colorBox = new QGroupBox( displays_.size(), Qt::Vertical, tools::translate( "GraphicPreferences", "Colors" ), parent );
    for( CIT_Displays it = displays_.begin(); it != displays_.end(); ++it )
        (*it)->Display( colorBox );
}

// -----------------------------------------------------------------------------
// Name: GraphicPreferences::SetAlpha
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
void GraphicPreferences::SetAlpha( float a )
{
    alpha_ = a;
}

// -----------------------------------------------------------------------------
// Name: GraphicPreferences::Commit
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
void GraphicPreferences::Commit()
{
    std::for_each( displays_.begin(), displays_.end(), boost::bind( &TerrainPreference::Commit, _1 ) );
    Save();
}

// -----------------------------------------------------------------------------
// Name: GraphicPreferences::Save
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
void GraphicPreferences::Save() const
{
    std::for_each( displays_.begin(), displays_.end(), boost::bind( &TerrainPreference::Save, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: GraphicPreferences::Revert
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
void GraphicPreferences::Revert()
{
    std::for_each( displays_.begin(), displays_.end(), boost::bind( &TerrainPreference::Revert, _1 ) );
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

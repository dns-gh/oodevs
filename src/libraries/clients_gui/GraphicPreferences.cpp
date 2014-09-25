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
#include "TerrainPreferenceWidget.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Options.h"
#include "clients_kernel/Tools.h"
#include <terrain/GraphicData.h>
#pragma warning( push, 0 )
#include <boost/algorithm/string.hpp>
#pragma warning( pop )

using namespace gui;

// -----------------------------------------------------------------------------
// Name: GraphicPreferences constructor
// Created: SBO 2006-04-04
// -----------------------------------------------------------------------------
GraphicPreferences::GraphicPreferences( kernel::Controllers& controllers )
    : controllers_     ( controllers )
    , alpha_           ( 1 )
    , upIcon_          ( "resources/images/gui/up.png" )
    , downIcon_        ( "resources/images/gui/down.png" )
    , upSignalMapper_  ( new QSignalMapper( this ) )
    , downSignalMapper_( new QSignalMapper( this ) )
    , layout_          ( 0 )
{
    tools::Xifstream xis( "preferences.xml" );
    xis >> xml::start( "preferences" )
            >> xml::start( "terrains" )
                >> xml::list( "terrain", *this, & GraphicPreferences::ReadTerrainPreference );
    connect( upSignalMapper_, SIGNAL( mapped( int ) ), this, SLOT( Up( int ) ) );
    connect( downSignalMapper_, SIGNAL( mapped( int ) ), this, SLOT( Down( int ) ) );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: GraphicPreferences destructor
// Created: SBO 2006-04-04
// -----------------------------------------------------------------------------
GraphicPreferences::~GraphicPreferences()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: GraphicPreferences::ReadTerrainPreference
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
void GraphicPreferences::ReadTerrainPreference( xml::xistream& xis )
{
    const std::string type = xis.attribute< std::string >( "type" );
    const std::string category = xis.attribute< std::string >( "category" );
    auto preference = std::make_shared< TerrainPreference >( xis, controllers_ );
    categories_[ category ].push_back( preference );
    if( std::find( currentOrders_.begin(), currentOrders_.end(), category ) == currentOrders_.end() )
        currentOrders_.push_back( category );
    terrainPrefs_[ TerrainData( type ) ] = preference;
}

// -----------------------------------------------------------------------------
// Name: GraphicPreferences::AddToPanel
// Created: SBO 2006-04-04
// -----------------------------------------------------------------------------
void GraphicPreferences::Display( QVBoxLayout* parent )
{
    previousOrders_ = currentOrders_;
    QGroupBox* colorBox = new QGroupBox( tools::translate( "gui::GraphicPreferences", "Colors and line thickness" ) );
    layout_ = new QVBoxLayout();
    colorBox->setLayout( layout_ );
    parent->addWidget( colorBox );
    Build();
}

// -----------------------------------------------------------------------------
// Name: GraphicPreferences::Up
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
void GraphicPreferences::Up( int index )
{
    if( index > 0 && static_cast< int >( currentOrders_.size() ) > index )
    {
        std::swap( currentOrders_[ index ], currentOrders_[ index - 1 ] );
        Build();
    }
}

// -----------------------------------------------------------------------------
// Name: GraphicPreferences::Down
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
void GraphicPreferences::Down( int index )
{
    if( index >= 0 && index < static_cast< int >( currentOrders_.size() ) - 1 )
    {
        std::swap( currentOrders_[ index ], currentOrders_[ index + 1 ] );
        Build();
    }
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
    previousOrders_ = currentOrders_;
    for( auto it = categories_.begin(); it != categories_.end(); ++it )
        std::for_each( it->second.begin(), it->second.end(), std::mem_fn( &TerrainPreference::Commit ) );
    Save();
}

// -----------------------------------------------------------------------------
// Name: GraphicPreferences::Save
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
void GraphicPreferences::Save() const
{
    for( auto it = categories_.begin(); it != categories_.end(); ++it )
        std::for_each( it->second.begin(), it->second.end(), std::mem_fn( &TerrainPreference::Save ) );

    QString order;
    for( auto it = currentOrders_.begin(); it != currentOrders_.end(); ++it )
    {
        if( it != currentOrders_.begin() )
            order += ";";
        order += it->c_str();
    }
    controllers_.options_.Change( "Terrains/Order", order );
}

// -----------------------------------------------------------------------------
// Name: GraphicPreferences::Revert
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
void GraphicPreferences::Revert()
{
    currentOrders_ = previousOrders_;
    for( auto it = categories_.begin(); it != categories_.end(); ++it )
        std::for_each( it->second.begin(), it->second.end(), std::mem_fn( &TerrainPreference::Revert ) );
    Build();
}

// -----------------------------------------------------------------------------
// Name: GraphicPreferences::SetupLineGraphics
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
void GraphicPreferences::SetupLineGraphics( const Data_ABC* pData )
{
    const GraphicData* d = static_cast< const GraphicData* >( pData );
    const TerrainData& data = *d;
    const auto preference = terrainPrefs_[ data ];
    if( preference )
    {
        preference->SetLineWidth();
        preference->SetColor( alpha_ );
    }
}

// -----------------------------------------------------------------------------
// Name: GraphicPreferences::SetupLineGraphics
// Created: FPT 2011-03-24
// -----------------------------------------------------------------------------
void GraphicPreferences::SetupLineGraphics( unsigned int /*offset*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GraphicPreferences::SetupBorderGraphics
// Created: AGE 2006-04-05
// -----------------------------------------------------------------------------
void GraphicPreferences::SetupBorderGraphics( const Data_ABC* pData )
{
    const GraphicData* d = static_cast< const GraphicData* >( pData );
    const TerrainData& data = *d;
    const auto preference = terrainPrefs_[ data ];
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
    const auto preference = terrainPrefs_[ data ];
    if( preference )
        preference->SetColor( alpha_ * 0.5f );
}

// -----------------------------------------------------------------------------
// Name: GraphicPreferences::OptionChanged
// Created: LGY 2014-06-20
// -----------------------------------------------------------------------------
void GraphicPreferences::OptionChanged( const std::string& name, const kernel::OptionVariant& value )
{
    const QString option( name.c_str() );
    if( option.startsWith( "Terrains/Order" ) )
    {
        const auto orders = value.To< QString >().toStdString();
        boost::split( previousOrders_, orders, boost::algorithm::is_any_of( ";" ) );
        currentOrders_ = previousOrders_;
        Build();
    }
}

namespace
{
    QLayoutItem* ClearLayoutItem( QLayoutItem* item )
    {
        if( auto layout = item->layout() )
            for( int i = 0; i < layout->count(); ++i )
                ClearLayoutItem( layout->itemAt( i ) );
        delete item->widget();
        return item;
    }

    void ClearLayout( QLayout* layout )
    {
        QLayoutItem* child;
        while( ( child = layout->takeAt( 0 ) ) != 0 )
            delete ClearLayoutItem( child );
    }
}

// -----------------------------------------------------------------------------
// Name: GraphicPreferences::Builds
// Created: LGY 2014-06-20
// -----------------------------------------------------------------------------
void GraphicPreferences::Build()
{
    ClearLayout( layout_ );
    for( int i = 0; i < currentOrders_.size(); ++i )
    {
        auto category = categories_.find( currentOrders_.at( i ) );
        if( category != categories_.end() )
        {
            const auto displayText = ENT_Tr::ConvertFromLocationCategory( ENT_Tr::ConvertToLocationCategory( category->first ), ENT_Tr::eToTr );
            layout_->insertWidget( i, new TerrainPreferenceWidget( displayText.c_str(), i == 0,
                i == currentOrders_.size() - 1, i, category->second, upSignalMapper_, downSignalMapper_ ) );
        }
    }
}

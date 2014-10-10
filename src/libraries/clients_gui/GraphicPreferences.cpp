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
#include "TerrainSetting.h"
#include "TerrainSettings.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/OptionsController.h"
#include "clients_kernel/Options.h"
#include "clients_kernel/OptionVariant.h"
#include "clients_kernel/Tools.h"
#include "ENT/Ent_Tr.h"
#include <xeumeuleu/xml.hpp>
#include <boost/bind.hpp>
#include <boost/make_shared.hpp>
#pragma warning( push, 0 )
#include <boost/algorithm/string.hpp>
#pragma warning( pop )

using namespace gui;

// -----------------------------------------------------------------------------
// Name: GraphicPreferences constructor
// Created: SBO 2006-04-04
// -----------------------------------------------------------------------------
GraphicPreferences::GraphicPreferences( kernel::OptionsController& options,
                                        const std::shared_ptr< TerrainSettings >& settings,
                                        QWidget* parent /* = 0 */ )
    : QWidget( parent )
    , options_         ( options )
    , settings_        ( settings )
    , upIcon_          ( "resources/images/gui/up.png" )
    , downIcon_        ( "resources/images/gui/down.png" )
    , upSignalMapper_  ( new QSignalMapper( this ) )
    , downSignalMapper_( new QSignalMapper( this ) )
    , layout_          ( new QVBoxLayout( this ) )
{
    setObjectName( "graphic-preferences" );
    layout_->setMargin( 0 );
    connect( upSignalMapper_, SIGNAL( mapped( QWidget* ) ), this, SLOT( Up( QWidget* ) ) );
    connect( downSignalMapper_, SIGNAL( mapped( QWidget* ) ), this, SLOT( Down( QWidget* ) ) );
}

// -----------------------------------------------------------------------------
// Name: GraphicPreferences destructor
// Created: SBO 2006-04-04
// -----------------------------------------------------------------------------
GraphicPreferences::~GraphicPreferences()
{
    // NOTHING
}

namespace
{
    QString FindCategory( const std::map< QString, TerrainPreferenceWidget* >& map, QWidget* widget )
    {
        for( auto it = map.begin(); it != map.end(); ++it )
            if( it->second == widget )
                return it->first;
        return "";
    }
}

// -----------------------------------------------------------------------------
// Name: GraphicPreferences::Swap
// Created: ABR 2014-10-08
// -----------------------------------------------------------------------------
void GraphicPreferences::Swap( int direction, QWidget* widget )
{
    int lhsIndex = orders_.indexOf( FindCategory( widgets_, widget ) );
    int rhsIndex = lhsIndex + direction;
    if( lhsIndex != -1 && rhsIndex >= 0 && rhsIndex < orders_.size() )
    {
        std::swap( orders_[ lhsIndex ], orders_[ rhsIndex ] );
        options_.Change( "Terrains/Order", orders_.join( ";" ) );
        Build();
    }
}

// -----------------------------------------------------------------------------
// Name: GraphicPreferences::Up
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
void GraphicPreferences::Up( QWidget* widget )
{
    Swap( -1, widget );
}

// -----------------------------------------------------------------------------
// Name: GraphicPreferences::Down
// Created: AGE 2007-02-23
// -----------------------------------------------------------------------------
void GraphicPreferences::Down( QWidget* widget )
{
    Swap( 1, widget );
}

// -----------------------------------------------------------------------------
// Name: GraphicPreferences::Builds
// Created: LGY 2014-06-20
// -----------------------------------------------------------------------------
void GraphicPreferences::Build()
{
    for( auto it = widgets_.begin(); it != widgets_.end(); ++it )
        layout_->removeWidget( it->second );
    for( int i = 0; i < orders_.size(); ++i )
    {
        auto widget = widgets_.at( orders_.at( i ) );
        layout_->insertWidget( i, widget );
        widget->UpdateUpDownButton( i != 0, i != orders_.size() - 1);
    }
}

// -----------------------------------------------------------------------------
// Name: GraphicPreferences::Load
// Created: ABR 2014-08-01
// -----------------------------------------------------------------------------
void GraphicPreferences::Load( const kernel::Options& options )
{
    orders_ = options.Get( "Terrains/Order" ).To< QString >().split( ";" );
    std::map< QString, std::vector< TerrainPreference* > > categories;
    for( auto it = widgets_.begin(); it != widgets_.end(); ++it )
        delete it->second;
    widgets_.clear();
    
    settings_->Apply( [&]( const std::shared_ptr< TerrainSetting >& setting ){
        categories[ setting->category_ ].push_back( new TerrainPreference( options_, setting ) );
    } );
    for( int i = 0; i < orders_.size(); ++i )
    {
        const auto category = orders_.at( i );
        const auto displayText = ENT_Tr::ConvertFromLocationCategory(
            ENT_Tr::ConvertToLocationCategory( category.toStdString() ), ENT_Tr::eToTr );
        widgets_[ category ] = new TerrainPreferenceWidget( displayText.c_str(), 
                                                            categories.at( category ),
                                                            upSignalMapper_,
                                                            downSignalMapper_ );
    }
    Build();
}

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

/* TRANSLATOR gui::GisToolbar */

#include "clients_gui_pch.h"
#include "GisToolbar.h"
#include "moc_GisToolbar.cpp"
#include "ColorButton.h"
#include "TerrainProfiler.h"
#include "Tools.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/DetectionMap.h"
#include "clients_kernel/Options.h"
#include "clients_kernel/OptionVariant.h"
#include "clients_kernel/Units.h"
#include "tools/GeneralConfig.h"

using namespace gui;

namespace
{
    QPixmap MakePixmap( const std::string& name )
    {
        return QPixmap( tools::GeneralConfig::BuildResourceChildFile( std::string( "images/gui/" ) + name + ".png" ).c_str() );
    }
}

// -----------------------------------------------------------------------------
// Name: GisToolbar constructor
// Created: SBO 2010-03-23
// -----------------------------------------------------------------------------
GisToolbar::GisToolbar( QMainWindow* parent, kernel::Controllers& controllers, const kernel::DetectionMap& detection, TerrainProfilerLayer& layer )
    : QToolBar( "gis tools", parent )
    , controllers_      ( controllers )
    , detection_        ( detection )
    , terrainProfiler_  ( new TerrainProfiler( parent, controllers, detection, layer ) )
{
    setObjectName( "gisToolBar" );
    parent->addDockWidget( Qt::RightDockWidgetArea, terrainProfiler_ );
    setWindowTitle( tools::translate( "gui::GisToolBar", "GIS tools" ) );
    {
        Q3HBox* waterShedBox = new Q3HBox( this );
        watershedEnabled_ = new QCheckBox( tools::translate( "gui::GisToolBar", "Watershed" ), waterShedBox );
        QToolTip::add( watershedEnabled_, tools::translate( "gui::GisToolBar", "Enable/disable watershed display" ) );
        mode_ = new QComboBox( waterShedBox );
        mode_->insertItem( tools::translate( "gui::GisToolBar", "<" ) );
        mode_->insertItem( tools::translate( "gui::GisToolBar", ">" ) );
        mode_->setMaximumWidth( 60 );
        QToolTip::add( mode_, tools::translate( "gui::GisToolBar", "Display water below or above specified height" ) );
        height_ = new QSpinBox( 0, 10000, 1, waterShedBox );
        height_->setSuffix( kernel::Units::meters.AsString() );
        height_->setEnabled( false );
        QToolTip::add( height_, tools::translate( "gui::GisToolBar", "Set water height limit" ) );
        color_ = new ColorButton( this );
        color_->SetColor( QColor( 20, 164, 218 ) ); // $$$$ SBO 2010-03-23: default from layer
        QToolTip::add( color_, tools::translate( "gui::GisToolBar", "Change watershed color" ) );

        connect( watershedEnabled_, SIGNAL( toggled( bool ) ), SLOT( OnToggleWatershedEnabled( bool ) ) );
        connect( mode_, SIGNAL( activated( int ) ), SLOT( OnModeChanged( int ) ) );
        connect( height_, SIGNAL( valueChanged( int ) ), SLOT( OnHeightChanged( int ) ) );
        connect( color_, SIGNAL( ColorChanged( const QColor& ) ), SLOT( OnColorChanged( const QColor& ) ) );

        QToolButton* button = new QToolButton( this );
        button->setIconSet( MakePixmap( "gis_terrainprofiler" ) );
        QToolTip::add( button, tools::translate( "gui::GisToolBar", "Show terrain profiler tool" ) );
        button->setToggleButton( true );
        connect( button, SIGNAL( toggled( bool ) ), SLOT( OnToggleCut( bool ) ) );
        connect( terrainProfiler_, SIGNAL( visibilityChanged( bool ) ), button, SLOT( setOn( bool ) ) );

        Q3HBox* contourBox = new Q3HBox( this );
        contourBoxEnabled_ = new QCheckBox( tools::translate( "gui::GisToolBar", "Contour lines" ), contourBox );
        QToolTip::add( contourBoxEnabled_, tools::translate( "gui::GisToolBar", "Enable/disable contour lines display" ) );
        colorContourLines_ = new ColorButton( this );
        colorContourLines_->SetColor( QColor( 245, 245, 220 ) ); // $$$$ SBO 2010-03-23: default from layer
        QToolTip::add( colorContourLines_, tools::translate( "gui::GisToolBar", "Change contour lines color" ) );
        connect( contourBoxEnabled_, SIGNAL( toggled( bool ) ), SLOT( OnToggleContourLinesEnabled( bool ) ) );
        connect( colorContourLines_, SIGNAL( ColorChanged( const QColor& ) ), SLOT( OnColorContourChanged( const QColor& ) ) );

        addWidget( watershedEnabled_ );
        addWidget( mode_ );
        addWidget( height_ );
        addWidget( color_ );
        addWidget( button );
        addWidget( contourBoxEnabled_ );
        addWidget( colorContourLines_ );
    }
    OnToggleWatershedEnabled( false );
    OnToggleContourLinesEnabled( false );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: GisToolbar destructor
// Created: SBO 2010-03-23
// -----------------------------------------------------------------------------
GisToolbar::~GisToolbar()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: GisToolbar::NotifyUpdated
// Created: SBO 2010-03-23
// -----------------------------------------------------------------------------
void GisToolbar::NotifyUpdated( const kernel::ModelLoaded& /*model*/ )
{
    height_->setMaxValue( detection_.MaximumElevation() );
}

// -----------------------------------------------------------------------------
// Name: GisToolbar::OptionChanged
// Created: SBO 2010-03-23
// -----------------------------------------------------------------------------
void GisToolbar::OptionChanged( const std::string& name, const kernel::OptionVariant& value )
{
    if( name == "WatershedEnabled" )
    {
        const bool val = value.To< bool >();
        if( val != watershedEnabled_->isChecked() )
            watershedEnabled_->setChecked( val );
    }
    else if( name == "WatershedHeight" )
    {
        const int val = value.To< int >();
        if( val != height_->value() )
            height_->setValue( val );
    }
    else if( name == "WatershedInverse" )
    {
        const int val = value.To< bool >() ? 1 : 0;
        if( val != mode_->currentItem() )
            mode_->setCurrentItem( val );
    }
    else if( name == "WatershedColor" )
    {
        QColor color( value.To< QString >() );
        if( color != color_->GetColor() )
            color_->SetColor( color );
        color_->update();
    }
    else if( name == "ContourLinesEnabled" )
    {
        const bool val = value.To< bool >();
        if( val != contourBoxEnabled_->isChecked() )
            contourBoxEnabled_->setChecked( val );
    }
    else if( name == "ContourLinesColor" )
    {
        QColor color( value.To< QString >() );
        if( color != colorContourLines_->GetColor() )
            colorContourLines_->SetColor( color );
        colorContourLines_->update();
    }
}

// -----------------------------------------------------------------------------
// Name: GisToolbar::OnToggleWatershedEnabled
// Created: SBO 2010-03-23
// -----------------------------------------------------------------------------
void GisToolbar::OnToggleWatershedEnabled( bool toggled )
{
    mode_->setEnabled( toggled );
    height_->setEnabled( toggled );
    color_->setEnabled( toggled );
    controllers_.options_.Change( "WatershedEnabled", toggled );
}

// -----------------------------------------------------------------------------
// Name: GisToolbar::OnToggleContourLinesEnabled
// Created: GGE 2011-06-23
// -----------------------------------------------------------------------------
void GisToolbar::OnToggleContourLinesEnabled( bool toggled )
{
    colorContourLines_->setEnabled( toggled );
    controllers_.options_.Change( "ContourLinesEnabled", toggled );
}

// -----------------------------------------------------------------------------
// Name: GisToolbar::OnModeChanged
// Created: SBO 2010-03-23
// -----------------------------------------------------------------------------
void GisToolbar::OnModeChanged( int mode )
{
    controllers_.options_.Change( "WatershedInverse", mode == 1 );
}

// -----------------------------------------------------------------------------
// Name: GisToolbar::OnHeightChanged
// Created: SBO 2010-03-23
// -----------------------------------------------------------------------------
void GisToolbar::OnHeightChanged( int height )
{
    controllers_.options_.Change( "WatershedHeight", height );
}

// -----------------------------------------------------------------------------
// Name: GisToolbar::OnColorContourChanged
// Created: SBO 2010-03-23
// -----------------------------------------------------------------------------
void GisToolbar::OnColorContourChanged( const QColor& color )
{
    controllers_.options_.Change( "ContourLinesColor", color.name() );
}

// -----------------------------------------------------------------------------
// Name: GisToolbar::OnColorChanged
// Created: SBO 2010-03-23
// -----------------------------------------------------------------------------
void GisToolbar::OnColorChanged( const QColor& color )
{
    controllers_.options_.Change( "WatershedColor", color.name() );
}

// -----------------------------------------------------------------------------
// Name: GisToolbar::OnToggleCut
// Created: SBO 2010-03-31
// -----------------------------------------------------------------------------
void GisToolbar::OnToggleCut( bool toggled )
{
    terrainProfiler_->setShown( toggled );
}

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

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
        return QImage( tools::GeneralConfig::BuildResourceChildFile( std::string( "images/gui/" ) + name + ".png" ).c_str() );
    }
}

// -----------------------------------------------------------------------------
// Name: GisToolbar constructor
// Created: SBO 2010-03-23
// -----------------------------------------------------------------------------
GisToolbar::GisToolbar( QMainWindow* parent, kernel::Controllers& controllers, const kernel::DetectionMap& detection, TerrainProfilerLayer& layer )
    : QToolBar( parent, "gis tools" )
    , controllers_( controllers )
    , detection_( detection )
    , terrainProfiler_( new TerrainProfiler( parent, controllers, detection, layer ) )
{
    setLabel( tools::translate( "GisToolBar", "GIS tools" ) );
    {
        QHBox* box = new QHBox( this );
        enabled_ = new QCheckBox( tools::translate( "GisToolBar", "Watershed" ), box );
        QToolTip::add( enabled_, tools::translate( "GisToolBar", "Enable/disable watershed display" ) );
        mode_ = new QComboBox( box );
        mode_->insertItem( tools::translate( "GisToolBar", "<" ) );
        mode_->insertItem( tools::translate( "GisToolBar", ">" ) );
        mode_->setMaximumWidth( 30 );
        QToolTip::add( mode_, tools::translate( "GisToolBar", "Display water below or above specified height" ) );
        height_ = new QSpinBox( 0, 10000, 1, box );
        height_->setSuffix( kernel::Units::meters.AsString() );
        height_->setEnabled( false );
        QToolTip::add( height_, tools::translate( "GisToolBar", "Set water height limit" ) );
        color_ = new ColorButton( this );
        color_->SetColor( QColor( 20, 164, 218 ) ); // $$$$ SBO 2010-03-23: default from layer
        QToolTip::add( color_, tools::translate( "GisToolBar", "Change watershed color" ) );

        connect( enabled_, SIGNAL( toggled( bool ) ), SLOT( OnToggleEnabled( bool ) ) );
        connect( mode_, SIGNAL( activated( int ) ), SLOT( OnModeChanged( int ) ) );
        connect( height_, SIGNAL( valueChanged( int ) ), SLOT( OnHeightChanged( int ) ) );
        connect( color_, SIGNAL( ColorChanged( const QColor& ) ), SLOT( OnColorChanged( const QColor& ) ) );

        addSeparator();
        QToolButton* button = new QToolButton( this );
        button->setIconSet( MakePixmap( "gis_terrainprofiler" ) );
        QToolTip::add( button, tools::translate( "GisToolBar", "Show terrain profiler tool" ) );
        button->setToggleButton( true );
        connect( button, SIGNAL( toggled( bool ) ), SLOT( OnToggleCut( bool ) ) );
        connect( terrainProfiler_, SIGNAL( visibilityChanged( bool ) ), button, SLOT( setOn( bool ) ) );
    }
    OnToggleEnabled( false );
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
        if( val != enabled_->isChecked() )
            enabled_->setChecked( val );
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
}

// -----------------------------------------------------------------------------
// Name: GisToolbar::OnToggleEnabled
// Created: SBO 2010-03-23
// -----------------------------------------------------------------------------
void GisToolbar::OnToggleEnabled( bool toggled )
{
    mode_->setEnabled( toggled );
    height_->setEnabled( toggled );
    color_->setEnabled( toggled );
    controllers_.options_.Change( "WatershedEnabled", toggled );
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

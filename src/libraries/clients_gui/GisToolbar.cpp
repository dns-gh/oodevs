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
#include "ContourLinesObserver.h"
#include "ImageWrapper.h"
#include "RichWidget.h"
#include "SubObjectName.h"
#include "TerrainProfiler.h"
#include "OptionWidgets.h"

#include "clients_kernel/Controllers.h"
#include "clients_kernel/DetectionMap.h"
#include "clients_kernel/DisplayableModesObserver_ABC.h"
#include "clients_kernel/OptionsController.h"
#include "clients_kernel/OptionVariant.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/Units.h"

#include "tools/GeneralConfig.h"

using namespace gui;

namespace
{
    QPixmap MakePixmap( const tools::Path& name )
    {
        return gui::Pixmap( tools::GeneralConfig::BuildResourceChildFile( tools::Path( "images/gui" ) / name + ".png" ) );
    }
}

// -----------------------------------------------------------------------------
// Name: GisToolbar constructor
// Created: SBO 2010-03-23
// -----------------------------------------------------------------------------
GisToolbar::GisToolbar( QMainWindow* parent,
                        kernel::Controllers& controllers,
                        const kernel::DetectionMap& detection,
                        gui::TerrainProfiler& terrainProfiler )
    : RichToolBar( controllers, parent, "gistoolbar", tr( "GIS tools" ) )
    , controllers_      ( controllers )
    , detection_        ( detection )
{
    SubObjectName subObject( "GisToolbar" );
    auto& options = controllers_.options_;

    // watershed
    auto watershedEnabled = new OptionCheckBox( options, "watershedEnabled", "Watershed/Enabled", tr( "Watershed" ) );
    QToolTip::add( watershedEnabled, tr( "Enable/disable watershed display" ) );

    mode_ = new RichWidget< QComboBox >( "mode" );
    mode_->insertItem( tr( "<" ) );
    mode_->insertItem( tr( ">" ) );
    mode_->setMaximumWidth( 60 );
    QToolTip::add( mode_, tr( "Display water below or above specified height" ) );

    watershedHeight_ = new OptionSpinBox( options, "height", "Watershed/Height", 0, 10000 );
    watershedHeight_->setSuffix( kernel::Units::meters.AsString() );
    QToolTip::add( watershedHeight_, tr( "Set water height limit" ) );
    watershedColor_ = new OptionColorButton( controllers_.options_, "color", "Watershed/Color" );
    QToolTip::add( watershedColor_, tr( "Change watershed color" ) );

    // terrain profiler
    terrainProfilerButton_ = new RichWidget< QToolButton >( "terrainProfilerButton", this );
    terrainProfilerButton_->setIconSet( MakePixmap( "gis_terrainprofiler" ) );
    QToolTip::add( terrainProfilerButton_, tr( "Show terrain profiler tool" ) );
    terrainProfilerButton_->setToggleButton( true );

    // contour lines
    auto contourEnabled = new OptionCheckBox( options, "contourBoxEnabled", "ContourLines/Enabled", tr( "Contour lines" ) );
    QToolTip::add( contourEnabled, tr( "Enable/disable contour lines display" ) );

    contourHeight_ = new OptionSpinBox( options, "linesHeight", "ContourLines/Height", 1, 10000 );
    contourHeight_->setSuffix( kernel::Units::meters.AsString() );
    QToolTip::add( contourHeight_, tr( "Set contour lines height" ) );
    contourColor_ = new OptionColorButton( options, "colorContourLines", "ContourLines/Color" );
    QToolTip::add( contourColor_, tr( "Change contour lines color" ) );

    progress_ = new QLabel();

    addWidget( watershedEnabled );
    addWidget( mode_ );
    addWidget( watershedHeight_ );
    addWidget( watershedColor_ );
    addWidget( terrainProfilerButton_ );
    addWidget( contourEnabled );
    addWidget( contourHeight_ );
    addWidget( contourColor_ );
    addWidget( progress_ );

    connect( watershedEnabled, SIGNAL( toggled( bool ) ), SLOT( OnToggleWatershedEnabled( bool ) ) );
    connect( mode_, SIGNAL( activated( int ) ), SLOT( OnModeChanged( int ) ) );
    connect( terrainProfilerButton_, SIGNAL( toggled( bool ) ), &terrainProfiler, SLOT( setVisible( bool ) ) );
    connect( &terrainProfiler, SIGNAL( visibilityChanged( bool ) ), this, SLOT( SetTerrainProfilerChecked( bool ) ) );
    connect( contourEnabled, SIGNAL( toggled( bool ) ), SLOT( OnToggleContourLinesEnabled( bool ) ) );

    controllers_.Update( *this );
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
    watershedHeight_->setMaxValue( detection_.MaximumElevation() );
}

// -----------------------------------------------------------------------------
// Name: GisToolbar::NotifyUpdated
// Created: JSR 2012-02-08
// -----------------------------------------------------------------------------
void GisToolbar::NotifyUpdated( const ContourLinesObserver& observer )
{
    short percentage = observer.GetPercentage();
    progress_->setText( percentage == 0 ? QString() : locale().toString( percentage ) + "% " );
}

// -----------------------------------------------------------------------------
// Name: GisToolbar::OptionChanged
// Created: SBO 2010-03-23
// -----------------------------------------------------------------------------
void GisToolbar::OptionChanged( const std::string& name, const kernel::OptionVariant& value )
{
    if( name == "Watershed/Inverse" )
    {
        const int val = value.To< bool >() ? 1 : 0;
        if( val != mode_->currentItem() )
            mode_->setCurrentItem( val );
    }
}

// -----------------------------------------------------------------------------
// Name: GisToolbar::OnModeChanged
// Created: SBO 2010-03-23
// -----------------------------------------------------------------------------
void GisToolbar::OnModeChanged( int mode )
{
    controllers_.options_.Change( "Watershed/Inverse", mode == 1 );
}

// -----------------------------------------------------------------------------
// Name: GisToolbar::OnToggleWatershedEnabled
// Created: SBO 2010-03-23
// -----------------------------------------------------------------------------
void GisToolbar::OnToggleWatershedEnabled( bool toggled )
{
    mode_->setEnabled( toggled );
    watershedHeight_->setEnabled( toggled );
    watershedColor_->setEnabled( toggled );
}

// -----------------------------------------------------------------------------
// Name: GisToolbar::OnToggleContourLinesEnabled
// Created: GGE 2011-06-23
// -----------------------------------------------------------------------------
void GisToolbar::OnToggleContourLinesEnabled( bool toggled )
{
    contourColor_->setEnabled( toggled );
    contourHeight_->setEnabled( toggled );
}

// -----------------------------------------------------------------------------
// Name: GisToolbar::SetTerrainProfilerChecked
// Created: LDC 2013-11-04
// -----------------------------------------------------------------------------
void GisToolbar::SetTerrainProfilerChecked( bool visible )
{
    bool wasBlocked = terrainProfilerButton_->blockSignals( true );
    terrainProfilerButton_->setChecked( visible );
    terrainProfilerButton_->blockSignals( wasBlocked );
}

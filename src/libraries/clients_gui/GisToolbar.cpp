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
#include "RichSpinBox.h"
#include "Tools.h"
#include "TerrainProfiler.h"
#include "SubObjectName.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/DetectionMap.h"
#include "clients_kernel/Options.h"
#include "clients_kernel/OptionVariant.h"
#include "clients_kernel/Units.h"
#include "clients_kernel/DisplayableModesObserver_ABC.h"
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
GisToolbar::GisToolbar( QMainWindow* parent, kernel::Controllers& controllers, const kernel::DetectionMap& detection, gui::TerrainProfiler& terrainProfiler )
    : RichToolBar( controllers, parent, "gistoolbar", tools::translate( "gui::GisToolBar", "GIS tools" ) )
    , controllers_      ( controllers )
    , detection_        ( detection )
{
    SubObjectName subObject( "GisToolbar" );
    {
        Q3HBox* waterShedBox = new Q3HBox( this );
        watershedEnabled_ = new RichCheckBox( "watershedEnabled", tools::translate( "gui::GisToolBar", "Watershed" ), waterShedBox );
        QToolTip::add( watershedEnabled_, tools::translate( "gui::GisToolBar", "Enable/disable watershed display" ) );
        mode_ = new RichWidget< QComboBox >( "mode", waterShedBox );
        mode_->insertItem( tools::translate( "gui::GisToolBar", "<" ) );
        mode_->insertItem( tools::translate( "gui::GisToolBar", ">" ) );
        mode_->setMaximumWidth( 60 );
        QToolTip::add( mode_, tools::translate( "gui::GisToolBar", "Display water below or above specified height" ) );
        height_ = new RichSpinBox( "height", waterShedBox, 0, 10000, 1 );
        height_->setSuffix( kernel::Units::meters.AsString() );
        height_->setEnabled( false );
        QToolTip::add( height_, tools::translate( "gui::GisToolBar", "Set water height limit" ) );
        color_ = new ColorButton( "color", this );
        color_->SetColor( QColor( 20, 164, 218 ) ); // $$$$ SBO 2010-03-23: default from layer
        QToolTip::add( color_, tools::translate( "gui::GisToolBar", "Change watershed color" ) );

        connect( watershedEnabled_, SIGNAL( toggled( bool ) ), SLOT( OnToggleWatershedEnabled( bool ) ) );
        connect( mode_, SIGNAL( activated( int ) ), SLOT( OnModeChanged( int ) ) );
        connect( height_, SIGNAL( valueChanged( int ) ), SLOT( OnHeightChanged( int ) ) );
        connect( color_, SIGNAL( ColorChanged( const QColor& ) ), SLOT( OnColorChanged( const QColor& ) ) );

        terrainProfilerButton_ = new RichWidget< QToolButton >( "terrainProfilerButton", this );
        terrainProfilerButton_->setIconSet( MakePixmap( "gis_terrainprofiler" ) );
        QToolTip::add( terrainProfilerButton_, tools::translate( "gui::GisToolBar", "Show terrain profiler tool" ) );
        terrainProfilerButton_->setToggleButton( true );

        connect( terrainProfilerButton_, SIGNAL( toggled( bool ) ), &terrainProfiler, SLOT( setVisible( bool ) ) );
        connect( &terrainProfiler, SIGNAL( visibilityChanged( bool ) ), terrainProfilerButton_, SLOT( setOn( bool ) ) );

        Q3HBox* contourBox = new Q3HBox( this );
        contourBoxEnabled_ = new RichCheckBox( "contourBoxEnabled", tools::translate( "gui::GisToolBar", "Contour lines" ), contourBox );
        QToolTip::add( contourBoxEnabled_, tools::translate( "gui::GisToolBar", "Enable/disable contour lines display" ) );

        linesHeight_ = new RichSpinBox( "linesHeight", contourBox, 1, 10000, 1 );
        linesHeight_->setValue( 100 );
        linesHeight_->setSuffix( kernel::Units::meters.AsString() );
        linesHeight_->setEnabled( false );
        QToolTip::add( linesHeight_, tools::translate( "gui::GisToolBar", "Set contour lines height" ) );
        colorContourLines_ = new ColorButton( "colorContourLines", this );
        colorContourLines_->SetColor( QColor( 245, 245, 220 ) ); // $$$$ SBO 2010-03-23: default from layer
        QToolTip::add( colorContourLines_, tools::translate( "gui::GisToolBar", "Change contour lines color" ) );

        progress_ = new QLabel( contourBox );

        connect( contourBoxEnabled_, SIGNAL( toggled( bool ) ), SLOT( OnToggleContourLinesEnabled( bool ) ) );
        connect( linesHeight_, SIGNAL( editingFinished() ), SLOT( OnLinesHeightChanged() ) );
        connect( colorContourLines_, SIGNAL( ColorChanged( const QColor& ) ), SLOT( OnColorContourChanged( const QColor& ) ) );

        addWidget( watershedEnabled_ );
        addWidget( mode_ );
        addWidget( height_ );
        addWidget( color_ );
        addWidget( terrainProfilerButton_ );
        addWidget( contourBoxEnabled_ );
        addWidget( linesHeight_ );
        addWidget( colorContourLines_ );
        addWidget( progress_ );
    }
    OnToggleWatershedEnabled( false );
    OnToggleContourLinesEnabled( false );
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
    height_->setMaxValue( detection_.MaximumElevation() );
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
    else if( name == "ContourLinesHeight" )
    {
        const int val = value.To< int >();
        if( val != linesHeight_->value() )
            linesHeight_->setValue( val );
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
    linesHeight_->setEnabled( toggled );
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
// Name: GisToolbar::OnLinesHeightChanged
// Created: JSR 2012-01-23
// -----------------------------------------------------------------------------
void GisToolbar::OnLinesHeightChanged()
{
    controllers_.options_.Change( "ContourLinesHeight", linesHeight_->value() );
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
// Name: GisToolbar::GetTerrainProfilerButton
// Created: ABR 2012-05-15
// -----------------------------------------------------------------------------
RichWidget< QToolButton >* GisToolbar::GetTerrainProfilerButton() const
{
    return terrainProfilerButton_;
}

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

/* TRANSLATOR gui::TerrainProfiler */

#include "clients_gui_pch.h"
#include "TerrainProfiler.h"
#include "moc_TerrainProfiler.cpp"
#include "TerrainProfile.h"
#include "TerrainProfilerLayer.h"
#include "Tools.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/DetectionMap.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/Units.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: TerrainProfiler constructor
// Created: SBO 2010-03-31
// -----------------------------------------------------------------------------
TerrainProfiler::TerrainProfiler( QMainWindow* parent, kernel::Controllers& controllers, const kernel::DetectionMap& detection, TerrainProfilerLayer& layer )
    : QDockWidget( "terrain-profiler", parent )
    , controllers_( controllers )
    , detection_  ( detection )
    , layer_      ( layer )
{
    setObjectName( "terrainProfiler" );
    setCaption( tools::translate( "gui::TerrainProfiler", "Terrain profile" ) );
    {
        Q3HBox* box = new Q3HBox( this );
        {
            Q3VBox* vbox = new Q3VBox( box );
            height_ = new QSlider( -100, 0, 1, 2, Qt::Vertical, vbox );
            height_->setTickmarks( QSlider::TicksRight );
            height_->setTickInterval( 10 );
            heightValue_ = new QSpinBox( 0, 100, 1, vbox );
            heightValue_->setSuffix( QString( " %1" ).arg( kernel::Units::meters.AsString() ) );
            vbox->setMaximumWidth( 50 );
        }
        profile_ = new TerrainProfile( box, detection );
        setWidget( box );
        connect( height_, SIGNAL( valueChanged( int ) ), SLOT( SliderChanged( int ) ) );
        connect( heightValue_, SIGNAL( valueChanged( int ) ), SLOT( SpinboxChanged( int ) ) );
    }
    parent->addDockWidget( Qt::RightDockWidgetArea, this );
    hide();
    setFloating( true );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: TerrainProfiler destructor
// Created: SBO 2010-03-31
// -----------------------------------------------------------------------------
TerrainProfiler::~TerrainProfiler()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: TerrainProfiler::NotifyContextMenu
// Created: SBO 2010-03-31
// -----------------------------------------------------------------------------
void TerrainProfiler::NotifyContextMenu( const geometry::Point2f& point, kernel::ContextMenu& menu )
{
    if( !isVisible() || detection_.Extent().IsOutside( point ) )
        return;
    candidatePoint_ = point;
    menu.InsertItem( "Helpers", tools::translate( "gui::TerrainProfiler", "Terrain profile from here" ), this, SLOT( SetFromPosition() ) );
    menu.InsertItem( "Helpers", tools::translate( "gui::TerrainProfiler", "Terrain profile to here" ), this, SLOT( SetToPosition() ) );
}

// -----------------------------------------------------------------------------
// Name: TerrainProfiler::NotifyContextMenu
// Created: SBO 2010-03-31
// -----------------------------------------------------------------------------
void TerrainProfiler::NotifyContextMenu( const kernel::Agent_ABC& entity, kernel::ContextMenu& menu )
{
    if( !isVisible() )
        return;
    candidateUnitPoint_ = entity.Get< kernel::Positions >().GetPosition();
    candidateHeight_ = entity.Get< kernel::Positions >().GetHeight();
    menu.InsertItem( "Helpers", tools::translate( "gui::TerrainProfiler", "Terrain profile from unit" ), this, SLOT( SetFromUnitPosition() ) );
    menu.InsertItem( "Helpers", tools::translate( "gui::TerrainProfiler", "Terrain profile to unit" ), this, SLOT( SetToUnitPosition() ) );
}

// -----------------------------------------------------------------------------
// Name: TerrainProfiler::NotifyUpdated
// Created: SBO 2010-04-02
// -----------------------------------------------------------------------------
void TerrainProfiler::NotifyUpdated( const kernel::ModelLoaded& /*model*/ )
{
    const short maxValue = detection_.MaximumElevation() + 100;
    height_->setRange( -maxValue, 0 );
    heightValue_->setRange( 0, maxValue );
    height_->setValue( 2 );
    heightValue_->setValue( 2 );
}

// -----------------------------------------------------------------------------
// Name: TerrainProfiler::SetFromPosition
// Created: SBO 2010-03-31
// -----------------------------------------------------------------------------
void TerrainProfiler::SetFromPosition()
{
    SetFromPosition( candidatePoint_ );
}

// -----------------------------------------------------------------------------
// Name: TerrainProfiler::SetToPosition
// Created: SBO 2010-03-31
// -----------------------------------------------------------------------------
void TerrainProfiler::SetToPosition()
{
    SetToPosition( candidatePoint_ );
}

// -----------------------------------------------------------------------------
// Name: TerrainProfiler::SetFromUnitPosition
// Created: SBO 2010-04-01
// -----------------------------------------------------------------------------
void TerrainProfiler::SetFromUnitPosition()
{
    SetFromPosition( candidateUnitPoint_, candidateHeight_ );
}

// -----------------------------------------------------------------------------
// Name: TerrainProfiler::SetToUnitPosition
// Created: SBO 2010-04-01
// -----------------------------------------------------------------------------
void TerrainProfiler::SetToUnitPosition()
{
    SetToPosition( candidateUnitPoint_ );
}

// -----------------------------------------------------------------------------
// Name: TerrainProfiler::SetFromPosition
// Created: SBO 2010-04-01
// -----------------------------------------------------------------------------
void TerrainProfiler::SetFromPosition( const geometry::Point2f& point, float height /* = 2.f*/ )
{
    from_ = point;
    height_->setValue( ( int )height );
    layer_.SetFromPosition( from_ );
    UpdateView();
}

// -----------------------------------------------------------------------------
// Name: TerrainProfiler::SetToPosition
// Created: SBO 2010-04-01
// -----------------------------------------------------------------------------
void TerrainProfiler::SetToPosition( const geometry::Point2f& point )
{
    to_ = point;
    layer_.SetToPosition( to_ );
    UpdateView();
}

// -----------------------------------------------------------------------------
// Name: TerrainProfiler::UpdateView
// Created: SBO 2010-03-31
// -----------------------------------------------------------------------------
void TerrainProfiler::UpdateView()
{
    if( !to_.IsZero() && !from_.IsZero() )
        profile_->Update( from_, to_, float( -height_->value() ) );
}

// -----------------------------------------------------------------------------
// Name: TerrainProfiler::showEvent
// Created: SBO 2010-03-31
// -----------------------------------------------------------------------------
void TerrainProfiler::showEvent( QShowEvent* /*e*/ )
{
    layer_.SetAlpha( 1.f );
}

// -----------------------------------------------------------------------------
// Name: TerrainProfiler::hideEvent
// Created: SBO 2010-03-31
// -----------------------------------------------------------------------------
void TerrainProfiler::hideEvent( QHideEvent* /*e*/ )
{
    layer_.SetAlpha( 0.f );
}

// -----------------------------------------------------------------------------
// Name: TerrainProfiler::SliderChanged
// Created: SBO 2010-04-02
// -----------------------------------------------------------------------------
void TerrainProfiler::SliderChanged( int value )
{
    heightValue_->blockSignals( true );
    heightValue_->setValue( -value );
    heightValue_->blockSignals( false );
    UpdateView();
}

// -----------------------------------------------------------------------------
// Name: TerrainProfiler::SpinboxChanged
// Created: SBO 2010-04-02
// -----------------------------------------------------------------------------
void TerrainProfiler::SpinboxChanged( int value )
{
    height_->blockSignals( true );
    height_->setValue( -value );
    height_->blockSignals( false );
    UpdateView();
}

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "TerrainProfiler.h"
#include "moc_TerrainProfiler.cpp"
#include "RichSpinBox.h"
#include "SubObjectName.h"
#include "TerrainProfile.h"
#include "TerrainProfilerLayer.h"

#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/DetectionMap.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/Units.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: TerrainProfiler constructor
// Created: SBO 2010-03-31
// -----------------------------------------------------------------------------
TerrainProfiler::TerrainProfiler( QMainWindow* parent, kernel::Controllers& controllers, const kernel::DetectionMap& detection, TerrainProfilerLayer& layer )
    : RichDockWidget( controllers, parent, "terrainProfiler", tools::translate( "gui::TerrainProfiler", "Terrain profile" ) )
    , controllers_( controllers )
    , detection_  ( detection )
    , layer_      ( layer )
{
    gui::SubObjectName subObject( this->objectName() );
    {
        QWidget* box = new QWidget( this );
        profile_ = new TerrainProfile( box, detection );
        QLabel* heightLabel = new QLabel( tools::translate( "gui::TerrainProfiler", "Observation height" ) );
        heightValue_ = new RichSpinBox( "heightValue", 0, 0 );
        heightValue_->setFixedSize( 100, 30 );
        heightValue_->setLineStep( 50 );
        heightValue_->setSuffix( QString( " %L1" ).arg( kernel::Units::meters.AsString() ) );
        QVBoxLayout* vlayout = new QVBoxLayout( box );
        vlayout->addWidget( profile_ );
        QHBoxLayout* hlayout = new QHBoxLayout( vlayout );
        hlayout->addWidget( heightLabel );
        hlayout->addWidget( heightValue_, 0, Qt::AlignLeft );
        hlayout->addStretch();
        setWidget( box );
        connect( heightValue_, SIGNAL( valueChanged( int ) ), SLOT( SpinboxChanged() ) );
    }
    setFloating( true );
    setVisible( false );
    controllers_.Update( *this );
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
    heightValue_->setValue( 0 );
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
    SetFromPosition( candidateUnitPoint_ );
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
void TerrainProfiler::SetFromPosition( const geometry::Point2f& point )
{
    from_ = point;
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
        profile_->Update( from_, to_, float( heightValue_->value() ) );
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
// Name: TerrainProfiler::SpinboxChanged
// Created: SBO 2010-04-02
// -----------------------------------------------------------------------------
void TerrainProfiler::SpinboxChanged()
{
    UpdateView();
}

// -----------------------------------------------------------------------------
// Name: TerrainProfiler::sizeHint
// Created: ABR 2012-05-16
// -----------------------------------------------------------------------------
QSize TerrainProfiler::sizeHint() const
{
    return QSize( 300, 100 );
}

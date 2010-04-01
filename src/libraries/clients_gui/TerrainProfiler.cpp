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
#include "TerrainProfile.h"
#include "TerrainProfilerLayer.h"
#include "Tools.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/DetectionMap.h"
#include "clients_kernel/Positions.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: TerrainProfiler constructor
// Created: SBO 2010-03-31
// -----------------------------------------------------------------------------
TerrainProfiler::TerrainProfiler( QMainWindow* parent, kernel::Controllers& controllers, const kernel::DetectionMap& detection, TerrainProfilerLayer& layer )
    : QDockWindow( parent, "terrain-profiler" )
    , controllers_( controllers )
    , detection_( detection )
    , layer_( layer )
    , profile_( new TerrainProfile( this, detection ) )
{
    setCaption( tools::translate( "TerrainProfiler", "Terrain profile" ) );
    setWidget( profile_ );
    setResizeEnabled( true );
    setCloseMode( QDockWindow::Always );
    undock();
    hide();
    parent->setAppropriate( this, false );
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
    {
        const int id = menu.InsertItem( "Helpers", tools::translate( "TerrainProfiler", "Terrain profile from here" ), this, SLOT( SetFromPosition() ) );
        menu.SetChecked( id, !from_.IsZero() );
    }
    {
        const int id = menu.InsertItem( "Helpers", tools::translate( "TerrainProfiler", "Terrain profile to here" ), this, SLOT( SetToPosition() ) );
        menu.SetChecked( id, !to_.IsZero() );
    }
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
    {
        const int id = menu.InsertItem( "Helpers", tools::translate( "TerrainProfiler", "Terrain profile from unit" ), this, SLOT( SetFromUnitPosition() ) );
        menu.SetChecked( id, !from_.IsZero() );
    }
    {
        const int id = menu.InsertItem( "Helpers", tools::translate( "TerrainProfiler", "Terrain profile to unit" ), this, SLOT( SetToUnitPosition() ) );
        menu.SetChecked( id, !to_.IsZero() );
    }
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
        profile_->Update( from_, to_ );
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

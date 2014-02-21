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
#include "Paths.h"
#include "RichSpinBox.h"
#include "SubObjectName.h"
#include "TerrainProfile.h"
#include "TerrainProfilerLayer.h"
#include "VisionLine.h"

#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/DetectionMap.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/Path.h"
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
        profile_ = new TerrainProfile( box );
        QLabel* heightLabel = new QLabel( tools::translate( "gui::TerrainProfiler", "Observation height" ) );
        heightValue_ = new RichSpinBox( "heightValue" );
        heightValue_->setFixedSize( 100, 30 );
        heightValue_->setLineStep( 50 );
        heightValue_->setSuffix( " " + kernel::Units::meters.AsString() );
        heightValue_->setValue( 1 );
        QLabel* slopeLabel = new QLabel( tools::translate( "gui::TerrainProfiler", "Slope threshold" ) );
        slopeValue_ = new RichSpinBox( "slopeValue", 0, 0, 90 );
        slopeValue_->setFixedSize( 100, 30 );
        slopeValue_->setLineStep( 1 );
        slopeValue_->setSuffix( " " + kernel::Units::degrees.AsString() );
        slopeValue_->setValue( 90 );
        QVBoxLayout* vlayout = new QVBoxLayout( box );
        vlayout->addWidget( profile_ );
        QHBoxLayout* hlayout = new QHBoxLayout( vlayout );
        hlayout->addWidget( heightLabel );
        hlayout->addWidget( heightValue_ );
        hlayout->addWidget( slopeLabel );
        hlayout->addWidget( slopeValue_ );
        hlayout->addStretch();
        setWidget( box );
        connect( heightValue_, SIGNAL( valueChanged( int ) ), SLOT( UpdateView() ) );
        connect( slopeValue_, SIGNAL( valueChanged( int ) ), SLOT( UpdateView() ) );
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
    if( const auto paths = entity.Retrieve< Paths >() )
        candidatePath_ = paths->GetPath();
    menu.InsertItem( "Helpers", tools::translate( "gui::TerrainProfiler", "Terrain profile from unit" ), this, SLOT( SetFromUnitPosition() ) );
    menu.InsertItem( "Helpers", tools::translate( "gui::TerrainProfiler", "Terrain profile to unit" ), this, SLOT( SetToUnitPosition() ) );
    if( !candidatePath_.empty() )
        menu.InsertItem( "Helpers", tools::translate( "gui::TerrainProfiler", "Terrain profile unit path" ), this, SLOT( SetPath() ) );
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
    path_.clear();
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
    path_.clear();
    to_ = point;
    layer_.SetToPosition( to_ );
    UpdateView();
}

void TerrainProfiler::SetPath()
{
    to_ = from_ = geometry::Point2f();
    path_.clear();
    path_.push_back( candidateUnitPoint_ );
    path_.insert( path_.end(), candidatePath_.begin(), candidatePath_.end() );
    from_ = path_.front();
    to_ = path_.back();
    layer_.SetFromPosition( from_ );
    layer_.SetToPosition( to_ );
    UpdateView();
}

// -----------------------------------------------------------------------------
// Name: TerrainProfiler::UpdateView
// Created: SBO 2010-03-31
// -----------------------------------------------------------------------------
void TerrainProfiler::UpdateView()
{
    if( path_.empty() )
        UpdatePointsView();
    else
        UpdatePathView();
}

void TerrainProfiler::UpdatePathView()
{
    std::vector< TerrainProfile::T_Point > points;
    auto previous = path_.begin();
    points.push_back( std::make_pair( 0, detection_.ElevationAt( *previous ) ) );
    double x = 0;
    for( auto it = previous + 1; it != path_.end(); previous = it++ )
    {
        x += previous->Distance( *it );
        points.push_back( std::make_pair( x / 1000, detection_.ElevationAt( *it ) ) );
    }
    profile_->Update( points, heightValue_->value(), slopeValue_->value() );
}

void TerrainProfiler::UpdatePointsView()
{
    if( to_.IsZero() || from_.IsZero() )
        return;
    std::vector< TerrainProfile::T_Point > points;
    double x = 0;
    const int height = heightValue_->value();
    VisionLine line( detection_, from_, to_, static_cast< float >( height ) );
    while( ! line.IsDone() )
    {
        line.Increment();
        const double value = line.Elevation() + ( x == 0 ? height : 0 );
        points.push_back( std::make_pair( x / 1000, value ) );
        x += line.Length();
    }
    profile_->Update( points, height, slopeValue_->value() );
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
// Name: TerrainProfiler::sizeHint
// Created: ABR 2012-05-16
// -----------------------------------------------------------------------------
QSize TerrainProfiler::sizeHint() const
{
    return QSize( 300, 100 );
}

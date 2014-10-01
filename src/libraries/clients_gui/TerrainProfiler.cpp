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
#include "RichCheckBox.h"
#include "RichSpinBox.h"
#include "SubObjectName.h"
#include "TerrainProfile.h"
#include "TerrainProfilerLayer.h"
#include "VisionLine.h"

#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/DetectionMap.h"
#include "clients_kernel/OptionVariant.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/Path.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/Units.h"
#include <boost/assign.hpp>

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
    , forestColor_( 170, 226, 164 )
    , suburbColor_( 195, 193, 194 )
{
    gui::SubObjectName subObject( this->objectName() );
    {
        QWidget* box = new QWidget( this );
        profile_ = new TerrainProfile( box, layer );
        heightCheckbox_ = new RichCheckBox( "heightCheckBox" );
        heightCheckbox_->setCheckState( Qt::Checked );
        QLabel* heightLabel = new QLabel( tools::translate( "gui::TerrainProfiler", "Observation height" ) );
        heightValue_ = new RichSpinBox( "heightValue" );
        heightValue_->setFixedSize( 100, 30 );
        heightValue_->setSuffix( " " + kernel::Units::meters.AsString() );
        heightValue_->setValue( 2 );
        slopeCheckbox_ = new RichCheckBox( "slopeCheckBox" );
        slopeCheckbox_->setCheckState( Qt::Checked );
        QLabel* slopeLabel = new QLabel( tools::translate( "gui::TerrainProfiler", "Slope threshold" ) );
        slopeValue_ = new RichSpinBox( "slopeValue", 0, 0, 90 );
        slopeValue_->setFixedSize( 100, 30 );
        slopeValue_->setLineStep( 1 );
        slopeValue_->setSuffix( " " + kernel::Units::degrees.AsString() );
        slopeValue_->setValue( 90 );
        QVBoxLayout* vlayout = new QVBoxLayout( box );
        vlayout->addWidget( profile_ );
        QHBoxLayout* hlayout = new QHBoxLayout( vlayout );
        hlayout->addWidget( heightCheckbox_ );
        hlayout->addWidget( heightLabel );
        hlayout->addWidget( heightValue_ );
        hlayout->addWidget( slopeCheckbox_ );
        hlayout->addWidget( slopeLabel );
        hlayout->addWidget( slopeValue_ );
        hlayout->addStretch();
        setWidget( box );
        connect( heightCheckbox_, SIGNAL( stateChanged( int ) ), SLOT( UpdateView() ) );
        connect( heightValue_, SIGNAL( valueChanged( int ) ), SLOT( UpdateView() ) );
        connect( slopeCheckbox_, SIGNAL( stateChanged( int ) ), SLOT( UpdateView() ) );
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
    heightValue_->setValue( 2 );
}

// -----------------------------------------------------------------------------
// Name: TerrainProfiler::OptionChanged
// Created: JSR 2014-06-17
// -----------------------------------------------------------------------------
void TerrainProfiler::OptionChanged( const std::string& name, const kernel::OptionVariant& value )
{
    static const QString forestOption = "Terrains/forest edge/color";
    static const QString suburbOption = "Terrains/suburb/color";
    if( name == forestOption.toStdString() )
    {
        const QColor newColor = QColor( value.To< QString >() );
        if( newColor != forestColor_ )
        {
            forestColor_ = newColor;
            UpdateView();
        }
    }
    else if( name == suburbOption.toStdString() )
    {
        const QColor newColor = QColor( value.To< QString >() );
        if( newColor != suburbColor_ )
        {
            suburbColor_= newColor;
            UpdateView();
        }
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
    if( !path_.empty() )
        Update( path_ );
    else if( !to_.IsZero() && !from_.IsZero() )
        Update( boost::assign::list_of( from_ )( to_ ) );
}

namespace
{
    const QColor forestColor( 170, 226, 164 );
    const QColor suburbColor( 195, 193, 194 );

    TerrainProfile::T_PointInfo CreatePointInfo( bool forest, bool town, short elevation, double x )
    {
        TerrainProfile::T_PointInfo info;
        info.height_ = forest ? 10.f : ( town ? 20.f : 0.f );
        info.color_ = forest ? forestColor : ( town ? suburbColor : QColor() );
        info.point_ = std::make_pair( x / 1000, elevation - info.height_ );
        return info;
    }
    void ComputePoints( std::vector< TerrainProfile::T_PointInfo >& points, const kernel::DetectionMap& detection,
        const geometry::Point2f& from, const geometry::Point2f& to, int height, double distance )
    {
        VisionLine line( detection, from, to, static_cast< float >( height ), true );
        double x = distance +line.CurrentPoint().Distance( from );
        while( !line.IsDone() )
        {
            points.push_back( CreatePointInfo( line.IsInForest(), line.IsInTown(), line.Elevation(), x ) );
            line.Increment();
            x += line.Length();
        }
    }
}

void TerrainProfiler::Update( const T_PointVector& path )
{
    if( path.size() > 1 )
    {
        const bool displayHeight = heightCheckbox_->checkState() == Qt::Checked;
        const bool displaySlope = slopeCheckbox_->checkState() == Qt::Checked;
        heightValue_->setEnabled( displayHeight );
        slopeValue_->setEnabled( displaySlope );
        std::vector< TerrainProfile::T_PointInfo > points;
        auto previous = path.begin();
        const auto environment = detection_.EnvironmentAt( *previous );
        const auto height = heightValue_->value();
        points.push_back( CreatePointInfo( environment.IsInForest(), environment.IsInTown(), static_cast< short >( detection_.ElevationAt( from_ ) ), 0 ) );
        double x = 0;
        for( auto it = previous + 1; it != path.end(); previous = it++ )
        {
            ComputePoints( points, detection_, *previous, *it, height, x );
            x += previous->Distance( *it );
        }
        profile_->Update( points, displayHeight, heightValue_->value(), displaySlope, slopeValue_->value(), path );
    }
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

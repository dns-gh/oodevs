// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "TerrainProfile.h"
#include "TerrainProfilerLayer.h"

#include "clients_kernel/Tools.h"
#include "clients_kernel/Units.h"
#include "clients_kernel/DetectionMap.h"
#include "clients_gui/GlTools_ABC.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: TerrainProfile constructor
// Created: SBO 2010-03-31
// -----------------------------------------------------------------------------
TerrainProfile::TerrainProfile( QWidget* parent, const std::shared_ptr< TerrainProfilerLayer >& layer )
    : gui::GQ_Plot( parent )
    , layer_        ( layer )
    , data_         ( new GQ_PlotData( 0, *this ) )
    , vision_       ( new GQ_PlotData( 1, *this ) )
    , slopes_       ( new GQ_PlotData( 2, *this ) )
    , selection_    ( new GQ_PlotData( 3, *this ) )
    , currentSlope_ ( 0 )
    , currentHeight_( 0 )
{
    setFocusPolicy( Qt::ClickFocus );
    YAxis().ShowAxis( true );
    YAxis().ShowGrid( true );
    YAxis().SetAxisCaption( tools::translate( "gui::TerrainProfile", "Altitude (m)" ) );
    YAxis().ShowTicks( 50 );
    YAxis().ShowTicksValue( true );
    YAxis().SetCaptionMargin( 8 );

    XAxis().ShowAxis( true );
    XAxis().ShowGrid( false );
    XAxis().ShowTicks( 1 );
    XAxis().ShowTicksValue( true );
    XAxis().SetAxisCaption( tools::translate( "gui::TerrainProfile", "Distance (km)" ) );

    SetBackgroundColor( Qt::white );
    setMinimumWidth( 320 );
    setMinimumHeight( 140 );
    setMouseTracking( true );

    data_->SetLinePen( QPen( QColor( 220, 220, 220 ), 3 ) );
    vision_->SetLinePen( QPen( Qt::blue, 1 ) );
    vision_->AddIgnoreValue( -1.f );
    slopes_->SetLinePen( QPen( Qt::red, 1 ) );
    slopes_->SetCanUpdateBBox( false );
    selection_->SetLinePen( QPen( Qt::darkMagenta, 1 ) );
    selection_->SetCanUpdateBBox( false );
    RegisterPlotData( *data_ );
    RegisterPlotData( *vision_ );
    RegisterPlotData( *slopes_ );
    RegisterPlotData( *selection_ );
}

// -----------------------------------------------------------------------------
// Name: TerrainProfile destructor
// Created: SBO 2010-03-31
// -----------------------------------------------------------------------------
TerrainProfile::~TerrainProfile()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TerrainProfile::Update
// Created: SBO 2010-03-31
// -----------------------------------------------------------------------------
void TerrainProfile::Update( const std::vector< T_PointInfo >& points, bool displayHeight, int height, bool displaySlope, int slope,
                             const std::vector< geometry::Point2f >& path )
{
    path_ = path;
    data_->ClearData();
    vision_->ClearData();
    slopes_->ClearData();
    selection_->ClearData();
    layer_->ClearCurrentPosition();
    for( auto it = points.begin(); it != points.end(); ++it )
    {
        data_->AddPoint( it->point_ );
        if( it->color_.isValid() )
            data_->AddColor( it->point_.first, it->color_ );
        if( it->height_ != 0 )
            data_->AddHeight( it->point_.first, it->height_ );
    }
    if( displayHeight )
        UpdateVision( height );
    if( displaySlope )
        UpdateSlopes( slope );
}

// -----------------------------------------------------------------------------
// Name: TerrainProfile::UpdateVision
// Created: SBO 2010-04-01
// -----------------------------------------------------------------------------
void TerrainProfile::UpdateVision( int height )
{
    const auto& data = data_->Data();
    if( data.size() < 2 )
        return;
    auto previous = data.begin();
    vision_->AddPoint( *previous );
    const geometry::Point2d viewer( 0, previous->second + height );
    geometry::Point2d maxpt( 0, previous->second );
    for( auto it = previous + 1; it != data.end(); previous = it++ )
    {
        const geometry::Point2d current( it->first, it->second );
        const geometry::Vector2d view2max( viewer, maxpt );
        const geometry::Vector2d view2current( viewer, current );
        if( view2max.CrossProduct( view2current ) >= 0 )
        {
            geometry::Point2d intersectpoint;
            const geometry::Line2d linemax( viewer, maxpt );
            const geometry::Line2d linecurrent( geometry::Point2d( previous->first, previous->second ), current );
            if( linemax.Intersect( linecurrent, intersectpoint )
                && intersectpoint.X() > previous->first
                && intersectpoint.X() < it->first )
                vision_->AddPoint( intersectpoint.X(), intersectpoint.Y() );
            vision_->AddPoint( *it );
            maxpt.Set( it->first, it->second );
        }
        else
            vision_->AddPoint( it->first, -1.f );
    }
}

namespace
{
    template< typename Point >
    int ComputeSlope( Point from, Point to )
    {
        static const double toDegrees = 360 / ( 2 * std::atan2( 0., -1. ) );
        const double slope = std::atan( ( to.second - from.second ) / ( to.first - from.first ) / 1000 ) * toDegrees;
        return static_cast< int >( slope );
    }
}

void TerrainProfile::UpdateSlopes( int threshold )
{
    const auto& data = data_->Data();
    if( data.size() < 2 )
        return;
    auto previous = data.begin();
    for( auto it = previous + 1; it != data.end(); previous = it++ )
    {
        const bool highlight = std::abs( ComputeSlope( *it, *previous ) ) > threshold;
        slopes_->AddPoint( previous->first, highlight ? previous->second : 0 );
        slopes_->AddPoint( it->first,  highlight ? it->second : 0 );
    }
}
namespace
{
    geometry::Point2f Project( const geometry::Point2f& from, const geometry::Point2f& to,
                                double distanceFrom, double distanceTo, double distanceCurrent )
    {
       return from + geometry::Vector2f( from, to ) *
            static_cast< float >( ( distanceCurrent - distanceFrom ) / ( distanceTo - distanceFrom ) );
    }
    geometry::Point2f Project( double distance, const std::vector< geometry::Point2f >& path )
    {
        double current = 0;
        auto previous = path.begin();
        for( auto it = previous + 1; it != path.end(); previous = it++ )
        {
            const auto nextDistance = current + previous->Distance( *it );
            if( distance < nextDistance )
                return Project( *previous, *it, current, nextDistance, distance );
            current = nextDistance;
        }
        return path.back();
    }
}

void TerrainProfile::mouseMoveEvent( QMouseEvent* event )
{
    const auto pos = MapFromViewport( event->pos() );
    const auto& data = data_->Data();
    if( data.size() >= 2 )
    {
        auto previous = data.end();
        auto next = data.begin();
        while( next != data.end() && next->first < pos.first )
            previous = next++;
        if( previous != data.end() && next != data.end() )
        {
            selection_->ClearData();
            selection_->AddPoint( pos.first, 0 );
            const auto b = ( previous->second * next->first  - next->second * previous->first ) / ( next->first - previous->first ) ;
            const auto a = ( next->second - b ) / ( next->first );
            layer_->SetCurrentPosition( Project( pos.first * 1000 , path_ ) );
            selection_->AddPoint( pos.first, YAxis().GetMaxAxisValue() );
            currentHeight_ = static_cast< int >( a * pos.first + b );
            currentSlope_ = ComputeSlope( *previous, *next );
            update();
        }
    }
    QToolTip::hideText();
    return QWidget::mouseMoveEvent( event );
}

bool TerrainProfile::event( QEvent* event )
{
    if( event->type() != QEvent::ToolTip )
        return QWidget::event( event );
    QHelpEvent* helpEvent = static_cast< QHelpEvent* >( event );
    QToolTip::showText( helpEvent->globalPos(),
        QString::number( currentSlope_ ) + kernel::Units::degrees.AsString() + "\n" +
        QString::number( currentHeight_ ) + kernel::Units::meters.AsString() );
    return true;
}

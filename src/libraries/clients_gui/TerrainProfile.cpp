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

#include "clients_kernel/Tools.h"
#include "clients_kernel/Units.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: TerrainProfile constructor
// Created: SBO 2010-03-31
// -----------------------------------------------------------------------------
TerrainProfile::TerrainProfile( QWidget* parent )
    : gui::GQ_Plot( parent )
    , data_  ( new GQ_PlotData( 0, *this ) )
    , vision_( new GQ_PlotData( 1, *this ) )
    , slopes_( new GQ_PlotData( 2, *this ) )
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

    data_->SetLinePen( QPen( QColor( 220, 220, 220 ), 3 ) );
    vision_->SetLinePen( QPen( Qt::blue, 1 ) );
    vision_->AddIgnoreValue( -1.f );
    slopes_->SetLinePen( QPen( Qt::red, 1 ) );
    RegisterPlotData( *data_ );
    RegisterPlotData( *vision_ );
    RegisterPlotData( *slopes_ );
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
void TerrainProfile::Update( const std::vector< T_Point >& points, int height, int slope )
{
    data_->ClearData();
    double x = 0;
    for( auto it = points.begin(); it != points.end(); ++it )
    {
        data_->AddPoint( *it );
        x = it->first;
    }
    UpdateVision( height );
    UpdateSlopes( slope );
}

// -----------------------------------------------------------------------------
// Name: TerrainProfile::UpdateVision
// Created: SBO 2010-04-01
// -----------------------------------------------------------------------------
void TerrainProfile::UpdateVision( int height )
{
    vision_->ClearData();
    const auto& data = data_->Data();
    if( data.size() < 2 )
        return;
    auto previous = data.begin();
    vision_->AddPoint( previous->first, previous->second + height );
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
    slopes_->ClearData();
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

bool TerrainProfile::event( QEvent* event )
{
    if( event->type() != QEvent::ToolTip )
        return QWidget::event( event );
    QHelpEvent* helpEvent = static_cast< QHelpEvent* >( event );
    const auto pos = MapFromViewport( helpEvent->pos() );
    const auto& data = data_->Data();
    if( data.size() >= 2 )
    {
        auto previous = data.end();
        auto next = data.begin();
        while( next != data.end() && next->first < pos.first )
            previous = next++;
        if( previous != data.end() && next != data.end() )
            QToolTip::showText( helpEvent->globalPos(),
                QString::number( ComputeSlope( *next, *previous ) ) + kernel::Units::degrees.AsString() );
        return true;
    }
    QToolTip::hideText();
    event->ignore();
    return true;
}

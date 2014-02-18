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
#include "VisionLine.h"

#include "clients_kernel/Tools.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: TerrainProfile constructor
// Created: SBO 2010-03-31
// -----------------------------------------------------------------------------
TerrainProfile::TerrainProfile( QWidget* parent, const kernel::DetectionMap& detection )
    : gui::GQ_Plot( parent )
    , detection_( detection )
    , data_     ( new GQ_PlotData( 0, *this ) )
    , vision_   ( new GQ_PlotData( 1, *this ) )
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
    RegisterPlotData( *data_ );
    RegisterPlotData( *vision_ );
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
void TerrainProfile::Update( const geometry::Point2f& from, const geometry::Point2f& to, float height )
{
    data_->ClearData();
    VisionLine line( detection_, from, to, height );
    float yMax = 0, x = 0;
    while( ! line.IsDone() )
    {
        line.Increment();
        const float value = line.Elevation() + ( x == 0 ? height : 0 );
        data_->AddPoint( x / 1000.f, value );
        yMax = std::max( value, yMax );
        x += line.Length();
    }
    YAxis().SetAxisRange( 0, yMax * 1.1f, true );
    XAxis().SetAxisRange( 0, x / 1000.f, true );

    UpdateVision( from, to, height );
}

// -----------------------------------------------------------------------------
// Name: TerrainProfile::UpdateVision
// Created: SBO 2010-04-01
// -----------------------------------------------------------------------------
void TerrainProfile::UpdateVision( const geometry::Point2f& from, const geometry::Point2f& to, float height )
{
    vision_->ClearData();
    VisionLine line( detection_, from, to, height );
    float yMax = -1, xMax = 0, x = 0;
    geometry::Point2f viewer, previous, current, maxpt;
    while( ! line.IsDone() )
    {
        line.Increment();
        const float value = line.Elevation() + ( x == 0 ? height : 0 );
        current = geometry::Point2f( x, value );
        if( x == 0 )
        {
            viewer = geometry::Point2f( 0, value );
            vision_->AddPoint( 0, value );
            yMax = value - height;
            maxpt = geometry::Point2f( xMax, yMax );
        }
        else
        {
            const geometry::Vector2f view2max    ( viewer, maxpt );
            const geometry::Vector2f view2current( viewer, current );
            if( view2max.CrossProduct( view2current ) >= 0 )
            {
                geometry::Point2f intersectpoint;
                const geometry::Line2f linemax    ( viewer, maxpt );
                const geometry::Line2f linecurrent( previous, current );
                if( linemax.Intersect( linecurrent, intersectpoint )
                    && ( intersectpoint.X() > previous.X() && intersectpoint.X() < x ) )
                    vision_->AddPoint( intersectpoint.X() /1000.f, intersectpoint.Y() );

                vision_->AddPoint( x / 1000.f, value );
                maxpt.Set( x, value );
            }
            else
                vision_->AddPoint( x / 1000.f, -1.f );
        }
        previous = geometry::Point2f( x, value );
        x += line.Length();
    }
}

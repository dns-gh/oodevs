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
#include "Tools.h"
#include "VisionLine.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: TerrainProfile constructor
// Created: SBO 2010-03-31
// -----------------------------------------------------------------------------
TerrainProfile::TerrainProfile( QWidget* parent, const kernel::DetectionMap& detection )
    : gui::GQ_Plot( parent )
    , detection_( detection )
    , data_( new GQ_PlotData( 0, *this ) )
{
    setFocusPolicy( QWidget::ClickFocus );
    YAxis().ShowAxis( true );
    YAxis().ShowGrid( true );
    YAxis().SetAxisCaption( tools::translate( "TerrainProfile", "Altitude (m)" ).ascii() );
    YAxis().ShowTicks( 50 );
    YAxis().ShowTicksValue( true );
    YAxis().SetCaptionMargin( 8 );

    XAxis().ShowAxis( true );
    XAxis().ShowGrid( false );
    XAxis().ShowTicks( 1 );
    XAxis().ShowTicksValue( true );
    XAxis().SetAxisCaption( tools::translate( "TerrainProfile", "Distance (km)" ).ascii() );

    SetBackgroundColor( Qt::white );
    setMinimumWidth( 320 );
    setMinimumHeight( 100 );

    QPen pen( Qt::blue, 2 );
    data_->SetLinePen( pen );
    RegisterPlotData( *data_ );
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
void TerrainProfile::Update( const geometry::Point2f& from, const geometry::Point2f& to )
{
    data_->ClearData();
    const float height = 2.f;
    VisionLine line( detection_, from, to, height );
    float yMax = 0, x = 0;
    while( ! line.IsDone() )
    {
        line.Increment();
        const float value = line.Elevation();
        data_->AddPoint( x / 1000.f, value );
        yMax = std::max( value, yMax );
        x += line.Length();
    }
    YAxis().SetAxisRange( 0, yMax * 1.1f, true );
    XAxis().SetAxisRange( 0, x / 1000.f, true );
}

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "StatisticsWidget.h"
#include "GQ_PlotData.h"

// -----------------------------------------------------------------------------
// Name: StatisticsWidget constructor
// Created: SBO 2007-01-04
// -----------------------------------------------------------------------------
StatisticsWidget::StatisticsWidget( QWidget* parent )
    : GQ_Plot( parent )
    , data_( new GQ_PlotData( 0, *this ) )
    , yMax_( 0 )
{
    YAxis().ShowAxis( true );
    YAxis().ShowGrid( true );
    YAxis().SetAxisRange( 0, 1, true );
    YAxis().ShowTicks( 1 );
    YAxis().ShowTicksValue( true );
    YAxis().SetAxisCaption( tr( "Rate (B)" ).ascii() );
    YAxis().SetCaptionMargin( 8 );

    XAxis().ShowAxis( true );
    XAxis().ShowGrid( true );
    XAxis().SetAxisRange( 0, 10, false );
    XAxis().ShowTicks( 1 );
    XAxis().ShowTicksValue( false );
    XAxis().SetAxisCaption( tr( "Time (Tick)" ).ascii() );

    SetBackgroundColor( Qt::white );
    setMinimumHeight( 100 );
    RegisterPlotData( *data_ );
}

// -----------------------------------------------------------------------------
// Name: StatisticsWidget destructor
// Created: SBO 2007-01-04
// -----------------------------------------------------------------------------
StatisticsWidget::~StatisticsWidget()
{
    UnregisterPlotData( *data_, true );
}

// -----------------------------------------------------------------------------
// Name: StatisticsWidget::AddValue
// Created: SBO 2007-01-04
// -----------------------------------------------------------------------------
void StatisticsWidget::AddValue( unsigned int tick, unsigned long value )
{
    lastValues_.push_back( value );
    if( lastValues_.size() > 10 )
        lastValues_.erase( lastValues_.begin() );

    T_Values::iterator itMax = std::max_element( lastValues_.begin(), lastValues_.end() );
    if( itMax != lastValues_.end() )
        YAxis().SetAxisRange( 0, *itMax, true );
    data_->AddPoint( tick, value );
    data_->SetDataRange( std::max( 0, (int)tick - 10 ) );
}


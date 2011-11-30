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
#include "clients_gui/GQ_PlotData.h"
#include "clients_kernel/Tools.h"

#pragma warning( disable : 4355 ) // $$$$ SBO 2008-05-14: 'this' : used in base member initializer list

// -----------------------------------------------------------------------------
// Name: StatisticsWidget constructor
// Created: SBO 2007-01-04
// -----------------------------------------------------------------------------
StatisticsWidget::StatisticsWidget( QWidget* parent )
    : gui::GQ_Plot( parent )
    , data_( new gui::GQ_PlotData( 0, *this ) )
    , yMax_( 0 )
    , visibleTicks_( 100 )
{
    YAxis().ShowAxis( true );
    YAxis().ShowGrid( true );
    YAxis().SetAxisRange( 0, 1, true );
    YAxis().ShowTicks( 10 );
    YAxis().ShowTicksValue( true );
    YAxis().SetAxisCaption( tools::translate( "StatisticsWidget", "Rate (B)" ).ascii() );
    YAxis().SetCaptionMargin( 8 );

    XAxis().ShowAxis( true );
    XAxis().ShowGrid( true );
    XAxis().SetAxisRange( 0, 1, false );
    XAxis().ShowTicks( 1 );
    XAxis().ShowTicksValue( true );
    XAxis().SetAxisCaption( tools::translate( "StatisticsWidget", "Time (Tick)" ).ascii() );

    SetBackgroundColor( Qt::white );
    setMinimumHeight( 130 );
    setMinimumWidth( 160 );
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
    if( lastValues_.size() > visibleTicks_ )
        lastValues_.erase( lastValues_.begin() );

    T_Values::iterator itMax = std::max_element( lastValues_.begin(), lastValues_.end() );
    if( itMax != lastValues_.end() )
        YAxis().SetAxisRange( 0, *itMax, true );
    data_->AddPoint( tick, value );
    data_->SetDataRange( std::max< int >( 0, tick - visibleTicks_ ) );
}

// -----------------------------------------------------------------------------
// Name: StatisticsWidget::SetYAxisCaption
// Created: SBO 2007-06-19
// -----------------------------------------------------------------------------
void StatisticsWidget::SetYAxisCaption( const QString& caption )
{
    YAxis().SetAxisCaption( caption.ascii() );
}

// -----------------------------------------------------------------------------
// Name: StatisticsWidget::SetXAxisCaption
// Created: SBO 2007-06-19
// -----------------------------------------------------------------------------
void StatisticsWidget::SetXAxisCaption( const QString& caption )
{
    XAxis().SetAxisCaption( caption.ascii() );
}

// -----------------------------------------------------------------------------
// Name: StatisticsWidget::SetVisibleTicks
// Created: SBO 2007-06-19
// -----------------------------------------------------------------------------
void StatisticsWidget::SetVisibleTicks( unsigned int count )
{
    visibleTicks_ = count;
}

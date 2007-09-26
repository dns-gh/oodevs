// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "AfterActionPlot.h"
#include "gaming/AfterActionRequest.h"

// -----------------------------------------------------------------------------
// Name: AfterActionPlot constructor
// Created: AGE 2007-09-26
// -----------------------------------------------------------------------------
AfterActionPlot::AfterActionPlot( QWidget* parent, QDockWindow* dock )
    : GQ_Plot( parent )
    , dock_( dock )
    , min_( 0 )
    , max_( -std::numeric_limits< double >::infinity() )
{
    YAxis().ShowAxis( true );
    YAxis().ShowGrid( true );
    YAxis().SetAxisCaption( tr( "Value" ).ascii() );
    YAxis().SetCaptionMargin( 8 );

    XAxis().ShowAxis( true );
    XAxis().ShowGrid( true );
    XAxis().SetAxisCaption( tr( "Time (Tick)" ).ascii() );

    SetBackgroundColor( Qt::white );
    setMinimumHeight( 130 );
    setMinimumWidth( 160 );
}

// -----------------------------------------------------------------------------
// Name: AfterActionPlot destructor
// Created: AGE 2007-09-26
// -----------------------------------------------------------------------------
AfterActionPlot::~AfterActionPlot()
{
    for( T_Datas::iterator it = datas_.begin(); it != datas_.end(); ++it )
        UnregisterPlotData( **it, true );
}

// -----------------------------------------------------------------------------
// Name: AfterActionPlot::Add
// Created: AGE 2007-09-26
// -----------------------------------------------------------------------------
void AfterActionPlot::Add( const AfterActionRequest& request )
{
    GQ_PlotData* data = new GQ_PlotData( datas_.size(), *this );

    for( unsigned i = 0; i < request.Result().size(); ++i )
    {
        double value = request.Result()[i];
        data->AddPoint( i, value );
        min_ = std::min( value, min_ );
        max_ = std::max( value, max_ );
    }
    YAxis().SetAxisRange( min_, max_* 1.1, true );
    XAxis().SetAxisRange( 0, request.Result().size(), true );
    
    RegisterPlotData( *data );
    datas_.push_back( data );
}

// -----------------------------------------------------------------------------
// Name: AfterActionPlot::hideEvent
// Created: AGE 2007-09-26
// -----------------------------------------------------------------------------
void AfterActionPlot::hideEvent( QHideEvent* event )
{
    GQ_Plot::hideEvent( event );
    dock_->deleteLater();
}

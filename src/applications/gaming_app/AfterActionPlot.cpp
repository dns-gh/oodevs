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
#include "gaming/ASN_Messages.h"
#include "gaming/Simulation.h"
#include "clients_kernel/Controllers.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AfterActionPlot constructor
// Created: AGE 2007-09-26
// -----------------------------------------------------------------------------
AfterActionPlot::AfterActionPlot( QWidget* parent, Controllers& controllers, Publisher_ABC& publisher, QDockWindow* dock )
    : GQ_Plot( parent )
    , controllers_( controllers )
    , publisher_( publisher )
    , dock_( dock )
    , tickData_( 0 )
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

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: AfterActionPlot destructor
// Created: AGE 2007-09-26
// -----------------------------------------------------------------------------
AfterActionPlot::~AfterActionPlot()
{
    controllers_.Unregister( *this );

    if( tickData_ )
        UnregisterPlotData( *tickData_, true );
    for( T_Datas::iterator it = datas_.begin(); it != datas_.end(); ++it )
        UnregisterPlotData( **it, true );
}

// -----------------------------------------------------------------------------
// Name: AfterActionPlot::Add
// Created: AGE 2007-09-26
// -----------------------------------------------------------------------------
void AfterActionPlot::Add( const AfterActionRequest& request )
{
    GQ_PlotData* data = new GQ_PlotData( datas_.size()+1, *this );

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

// -----------------------------------------------------------------------------
// Name: AfterActionPlot::mouseReleaseEvent
// Created: AGE 2007-09-27
// -----------------------------------------------------------------------------
void AfterActionPlot::mouseReleaseEvent( QMouseEvent* e )
{
    const int offset = YAxis().GetAxisSize().width() + YAxis().GetCaptionSize().width();
    double tick = XAxis().MapFromViewport( e->pos().x() - offset );
    ASN_MsgReplayControlSkipToTick skip;
    skip() = unsigned( tick );
    skip.Send( publisher_ );

    GQ_Plot::mouseReleaseEvent( e );
}

// -----------------------------------------------------------------------------
// Name: AfterActionPlot::resizeEvent
// Created: AGE 2007-09-27
// -----------------------------------------------------------------------------
void AfterActionPlot::resizeEvent( QResizeEvent* e )
{
    GQ_Plot::resizeEvent( e );
    UpdateAxis();
}

// -----------------------------------------------------------------------------
// Name: AfterActionPlot::NotifyUpdated
// Created: AGE 2007-09-27
// -----------------------------------------------------------------------------
void AfterActionPlot::NotifyUpdated( const Simulation& simulation )
{
    if( ! tickData_ )
    {
        tickData_ = new GQ_PlotData( 0, *this );
        RegisterPlotData( *tickData_ );
    }
    tickData_->SetLinePen( red );
    tickData_->ClearData();
    tickData_->AddPoint( simulation.GetCurrentTick(), min_       );
    tickData_->AddPoint( simulation.GetCurrentTick(), max_ * 1.1 );
}

// -----------------------------------------------------------------------------
// Name: AfterActionPlot::UpdateAxis
// Created: AGE 2007-09-27
// -----------------------------------------------------------------------------
void AfterActionPlot::UpdateAxis()
{

}

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "IndicatorPlot.h"
#include "moc_IndicatorPlot.cpp"
#include "game_asn/ReplaySenders.h"
#include "gaming/IndicatorRequest.h"
#include "gaming/Simulation.h"
#include "gaming/Tools.h"
#include "clients_kernel/Controllers.h"
#include "clients_gui/ValuedDragObject.h"
#include <boost/foreach.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: IndicatorPlot constructor
// Created: AGE 2007-09-26
// -----------------------------------------------------------------------------
IndicatorPlot::IndicatorPlot( QWidget* parent, Controllers& controllers, Publisher_ABC& publisher, QDockWindow* dock, bool interactive )
    : GQ_Plot( parent, "IndicatorPlot" )
    , controllers_( controllers )
    , publisher_( publisher )
    , interactive_( interactive )
    , dock_( dock )
    , tickData_( 0 )
    , min_( 0 )
    , max_( -std::numeric_limits< double >::infinity() )
{
    setAcceptDrops( true );
    setFocusPolicy( QWidget::ClickFocus );
    YAxis().ShowAxis( true );
    YAxis().ShowGrid( true );
    YAxis().SetAxisCaption( tools::translate( "IndicatorPlot", "Value" ).ascii() );
    YAxis().ShowTicks( 25 );
    YAxis().ShowTicksValue( true );
    YAxis().SetCaptionMargin( 8 );

    XAxis().ShowAxis( true );
    XAxis().ShowGrid( true );
    XAxis().ShowTicks( 25 );
    XAxis().ShowTicksValue( true );
    XAxis().SetAxisCaption( tools::translate( "IndicatorPlot","Time (Tick)" ).ascii() );

    SetBackgroundColor( Qt::white );
    setMinimumWidth( 320 );
    setMinimumHeight( 200 );

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: IndicatorPlot destructor
// Created: AGE 2007-09-26
// -----------------------------------------------------------------------------
IndicatorPlot::~IndicatorPlot()
{
    controllers_.Unregister( *this );

    if( tickData_ )
        UnregisterPlotData( *tickData_, true );
    for( T_Datas::iterator it = datas_.begin(); it != datas_.end(); ++it )
        UnregisterPlotData( **it, true );
}

namespace
{
    QColor GetPlotColor( unsigned i )
    {
        switch( i % 7 )
        {
        default:
        case 0: return Qt::black;
        case 1: return Qt::green;
        case 2: return Qt::blue;
        case 3: return Qt::cyan;
        case 4: return Qt::magenta;
        case 5: return Qt::yellow;
        case 6: return Qt::darkYellow;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: IndicatorPlot::Add
// Created: AGE 2007-09-26
// -----------------------------------------------------------------------------
void IndicatorPlot::Add( const IndicatorRequest& request )
{
    GQ_PlotData* data = new GQ_PlotData( datas_.size() + 1, *this );
    data->SetLinePen( GetPlotColor( datas_.size() ) );
    UpdatePlot( data, request, 0 );
    RegisterPlotData( *data );
    datas_.push_back( data );
    plots_[ &request ] = data;
}

// -----------------------------------------------------------------------------
// Name: IndicatorPlot::NotifyUpdated
// Created: SBO 2009-04-30
// -----------------------------------------------------------------------------
void IndicatorPlot::NotifyUpdated( const IndicatorRequest& request )
{
    T_PlottedRequests::const_iterator it = plots_.find( &request );
    if( it != plots_.end() )
    {
        GQ_PlotData* data = it->second;
        if( data->Data().size() < request.Result().size() )
            UpdatePlot( data, request, data->Data().size() - 1 );
        else if( data->Data().size() > request.Result().size() )
        {
            data->ClearData();
            UpdatePlot( data, request, 0 );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: IndicatorPlot::UpdatePlot
// Created: SBO 2009-04-30
// -----------------------------------------------------------------------------
void IndicatorPlot::UpdatePlot( GQ_PlotData* plot, const IndicatorRequest& request, unsigned int from )
{
    for( unsigned int i = from; i < request.Result().size(); ++i )
    {
        const double value = request.Result()[i];
        plot->AddPoint( i, value );
        min_ = std::min( value, min_ );
        max_ = std::max( value, max_ );
    }
    YAxis().SetAxisRange( min_, max_* 1.1, true );
    XAxis().SetAxisRange( 0, request.Result().size(), true );
}

// -----------------------------------------------------------------------------
// Name: IndicatorPlot::hideEvent
// Created: AGE 2007-09-26
// -----------------------------------------------------------------------------
void IndicatorPlot::hideEvent( QHideEvent* event )
{
    GQ_Plot::hideEvent( event );
    dock_->deleteLater();
}

// -----------------------------------------------------------------------------
// Name: IndicatorPlot::mouseReleaseEvent
// Created: AGE 2007-09-27
// -----------------------------------------------------------------------------
void IndicatorPlot::mouseReleaseEvent( QMouseEvent* e )
{
    if( interactive_ )
    {
        const int offset = YAxis().GetAxisSize().width() + YAxis().GetCaptionSize().width();
        double tick = XAxis().MapFromViewport( e->pos().x() - offset );
        replay::ControlSkipToTick skip;
        skip() = unsigned( tick );
        skip.Send( publisher_ );
    }
    GQ_Plot::mouseReleaseEvent( e );
}

// -----------------------------------------------------------------------------
// Name: IndicatorPlot::contextMenuEvent
// Created: SBO 2009-04-30
// -----------------------------------------------------------------------------
void IndicatorPlot::contextMenuEvent( QContextMenuEvent* e )
{
    QPopupMenu* menu = new QPopupMenu( this );
    menu->insertItem( tr( "Refresh" ), this, SLOT( OnRefresh() ), Qt::Key_F5 );
    menu->exec( e->globalPos() );
}

// -----------------------------------------------------------------------------
// Name: IndicatorPlot::keyPressEvent
// Created: SBO 2009-04-30
// -----------------------------------------------------------------------------
void IndicatorPlot::keyPressEvent( QKeyEvent* e )
{
    if( e->key() == Qt::Key_F5 )
        OnRefresh();
    GQ_Plot::keyPressEvent( e );
}

// -----------------------------------------------------------------------------
// Name: IndicatorPlot::OnRefresh
// Created: SBO 2009-04-30
// -----------------------------------------------------------------------------
void IndicatorPlot::OnRefresh()
{
    BOOST_FOREACH( const T_PlottedRequests::value_type& plot, plots_ )
        plot.first->Commit();
}

// -----------------------------------------------------------------------------
// Name: IndicatorPlot::NotifyUpdated
// Created: AGE 2007-09-27
// -----------------------------------------------------------------------------
void IndicatorPlot::NotifyUpdated( const Simulation& simulation )
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
// Name: IndicatorPlot::dragEnterEvent
// Created: AGE 2007-09-27
// -----------------------------------------------------------------------------
void IndicatorPlot::dragEnterEvent( QDragEnterEvent* e )
{
    e->accept( gui::ValuedDragObject::Provides< const IndicatorRequest >( e ) );
}

// -----------------------------------------------------------------------------
// Name: IndicatorPlot::dropEvent
// Created: AGE 2007-09-27
// -----------------------------------------------------------------------------
void IndicatorPlot::dropEvent( QDropEvent* e )
{
    if( const IndicatorRequest* request = gui::ValuedDragObject::GetValue< const IndicatorRequest >( e ) )
        Add( *request );
}

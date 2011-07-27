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
#include "IndicatorExportDialog.h"
#include "gaming/IndicatorRequest.h"
#include "gaming/Simulation.h"
#include "gaming/Tools.h"
#include "clients_kernel/Controllers.h"
#include "clients_gui/ValuedDragObject.h"
#include "protocol/Protocol.h"
#include "protocol/ReplaySenders.h"
#include <boost/foreach.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: IndicatorPlot constructor
// Created: AGE 2007-09-26
// -----------------------------------------------------------------------------
IndicatorPlot::IndicatorPlot( QWidget* parent, Controllers& controllers, Publisher_ABC& publisher, QDockWindow* dock,
                              IndicatorExportDialog& exportDialog, bool interactive, const IndicatorRequest& request, double currentTick,
                              QVBox* vbox )
    : gui::GQ_Plot( parent, "IndicatorPlot" )
    , controllers_ ( controllers )
    , publisher_   ( publisher )
    , interactive_ ( interactive )
    , dock_        ( dock )
    , exportDialog_( exportDialog )
    , tickData_    ( 0 )
    , min_         ( 0 )
    , max_         ( -std::numeric_limits< double >::infinity() )
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
    setMinimumHeight( 250 );
    controllers_.Register( *this );
    plotNames_ = new QLabel( vbox );
    plotNames_->setBackgroundColor( Qt::white );
    plotNames_->setIndent( 20 );
    Add( request );
    SetTickData( currentTick );
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
    QColor GetPlotColor( unsigned int i )
    {
        QColor col;
        const int nColors = 6;
        int dividend = i / nColors;
        if( dividend < 3 )
        {
            switch( i % nColors )
            {
                case 0: col = Qt::blue; break;
                case 1: col = Qt::red; break;
                case 2: col = Qt::magenta; break;
                case 3: col = Qt::gray; break;
                case 4: col = Qt::green; break;
                case 5: col = Qt::cyan; break;
                default : assert( false );
            }
            if( dividend == 1 )
                col = col.light( 166 );
            else if( dividend == 2 )
                col = col.light( 33 );
        }
        else
            col.setHsv( ( i * 43 ) % 360, 100, 100 );
        return col;
    }
}

// -----------------------------------------------------------------------------
// Name: IndicatorPlot::Add
// Created: AGE 2007-09-26
// -----------------------------------------------------------------------------
void IndicatorPlot::Add( const IndicatorRequest& request )
{
    T_PlottedRequests::const_iterator it = plots_.find( &request );
    if( it != plots_.end() )
        return;
    gui::GQ_PlotData* data = new gui::GQ_PlotData( static_cast< unsigned >( datas_.size() + 1 ), *this );
    plotNames_->setText( plotNames_->text() + "<font color='" + GetPlotColor( static_cast< unsigned >( datas_.size() ) ).name() + "'>" + request.GetName() + "</font>" + "<br />" );
    plotNames_->setMaximumHeight( datas_.size() * ( plotNames_->fontMetrics().height() + 10 ) + 30 );
    data->SetLinePen( GetPlotColor( static_cast< unsigned >( datas_.size() ) ) );
    UpdatePlot( data, request, 0 );
    RegisterPlotData( *data );
    datas_.push_back( data );
    plots_[ &request ] = data;
    if( plots_.size() > 1 )
        dock_->setCaption( QString( "%1 - %2" ).arg( dock_->caption() ).arg( request.GetName() ) );
    else
        dock_->setCaption( request.GetName() );
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
        gui::GQ_PlotData* data = it->second;
        if( data->Data().size() < request.Result().size() )
            UpdatePlot( data, request, static_cast< unsigned >( data->Data().size() - 1 ) );
        else if( data->Data().size() > request.Result().size() )
        {
            data->ClearData();
            UpdatePlot( data, request, 0 );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: IndicatorPlot::NotifyDeleted
// Created: SBO 2009-06-12
// -----------------------------------------------------------------------------
void IndicatorPlot::NotifyDeleted( const IndicatorRequest& request )
{
    T_PlottedRequests::iterator it = plots_.find( &request );
    if( it != plots_.end() )
    {
        gui::GQ_PlotData* data = it->second;
        UnregisterPlotData( *data, true );
        T_Datas::iterator itD = std::find( datas_.begin(), datas_.end(), data );
        datas_.erase( itD );
        plots_.erase( it );
    }
    if( plots_.empty() )
        dock_->deleteLater();
}

// -----------------------------------------------------------------------------
// Name: IndicatorPlot::UpdatePlot
// Created: SBO 2009-04-30
// -----------------------------------------------------------------------------
void IndicatorPlot::UpdatePlot( gui::GQ_PlotData* plot, const IndicatorRequest& request, unsigned int from )
{
    const unsigned int firstTick = request.GetFirstTick();
    for( unsigned int i = from; i < request.Result().size(); ++i )
    {
        const double value = request.Result()[i];
        plot->AddPoint( firstTick + i, value );
        min_ = std::min( value, min_ );
        max_ = std::max( value, max_ );
    }
    YAxis().SetAxisRange( min_, max_ * 1.1, true );
    XAxis().SetAxisRange( firstTick, static_cast< double >( firstTick + request.Result().size() ), true );
}

// -----------------------------------------------------------------------------
// Name: IndicatorPlot::hideEvent
// Created: AGE 2007-09-26
// -----------------------------------------------------------------------------
void IndicatorPlot::hideEvent( QHideEvent* event )
{
    gui::GQ_Plot::hideEvent( event );
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
        skip().set_tick( static_cast< unsigned int >( tick ) );
        skip.Send( publisher_ );
    }
    gui::GQ_Plot::mouseReleaseEvent( e );
}

// -----------------------------------------------------------------------------
// Name: IndicatorPlot::contextMenuEvent
// Created: SBO 2009-04-30
// -----------------------------------------------------------------------------
void IndicatorPlot::contextMenuEvent( QContextMenuEvent* e )
{
    QPopupMenu* menu = new QPopupMenu( this );
    menu->insertItem( tools::translate( "Indicators", "Export data..." ), this, SLOT( OnExportData() ) );
    menu->exec( e->globalPos() );
}

// -----------------------------------------------------------------------------
// Name: IndicatorPlot::OnExportData
// Created: SBO 2009-05-04
// -----------------------------------------------------------------------------
void IndicatorPlot::OnExportData()
{
    BOOST_FOREACH( const T_PlottedRequests::value_type& plot, plots_ )
        exportDialog_.Add( *plot.first );
    exportDialog_.Export();
}

// -----------------------------------------------------------------------------
// Name: IndicatorPlot::NotifyUpdated
// Created: AGE 2007-09-27
// -----------------------------------------------------------------------------
void IndicatorPlot::NotifyUpdated( const Simulation& simulation )
{
    SetTickData( simulation.GetCurrentTick() );
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

// -----------------------------------------------------------------------------
// Name: IndicatorPlot::SetTickData
// Created: FPO 2011-03-23
// -----------------------------------------------------------------------------
void IndicatorPlot::SetTickData( double currentTickPosition )
{
    if( !tickData_ )
    {
        tickData_ = new gui::GQ_PlotData( 0, *this );
        RegisterPlotData( *tickData_ );
    }
    tickData_->SetLinePen( red );
    tickData_->ClearData();
    if( min_ == 0 && max_ == 0 )
    {
        tickData_->AddPoint( currentTickPosition, 0 );
        tickData_->AddPoint( currentTickPosition, 1 );
    }
    else
    {
        tickData_->AddPoint( currentTickPosition, min_ );
        tickData_->AddPoint( currentTickPosition, max_ * 1.1 );
    }
}

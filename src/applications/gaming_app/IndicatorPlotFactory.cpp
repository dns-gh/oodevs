// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "IndicatorPlotFactory.h"
#include "IndicatorPlot.h"
#include "clients_kernel/Controllers.h"
#include "game_asn/ReplaySenders.h"
#include "gaming/Services.h"
#include "gaming/Tools.h"

// -----------------------------------------------------------------------------
// Name: IndicatorPlotFactory constructor
// Created: SBO 2009-04-30
// -----------------------------------------------------------------------------
IndicatorPlotFactory::IndicatorPlotFactory( QMainWindow* mainWindow, kernel::Controllers& controllers, Publisher_ABC& publisher, IndicatorExportDialog& exportDialog )
    : mainWindow_( mainWindow )
    , controllers_( controllers )
    , publisher_( publisher )
    , exportDialog_( exportDialog )
    , hasReplay_( false )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: IndicatorPlotFactory destructor
// Created: SBO 2009-04-30
// -----------------------------------------------------------------------------
IndicatorPlotFactory::~IndicatorPlotFactory()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: IndicatorPlotFactory::CreatePlot
// Created: SBO 2009-04-30
// -----------------------------------------------------------------------------
IndicatorPlot* IndicatorPlotFactory::CreatePlot( const IndicatorRequest& request )
{
    QDockWindow* dock = new QDockWindow( mainWindow_, "indicatorplot", Qt::WDestructiveClose );
    QVBox* box = new QVBox( dock );
    IndicatorPlot* plot = new IndicatorPlot( box, controllers_, publisher_, dock, exportDialog_, hasReplay_ );
    dock->setWidget( box );
    dock->setResizeEnabled( true );
    dock->setCloseMode( QDockWindow::Always );
    dock->undock();
    mainWindow_->setAppropriate( dock, false );
    box->show();
    plot->Add( request );
    return plot;
}

// -----------------------------------------------------------------------------
// Name: IndicatorPlotFactory::NotifyUpdated
// Created: SBO 2009-04-30
// -----------------------------------------------------------------------------
void IndicatorPlotFactory::NotifyUpdated( const Services& services )
{
    hasReplay_ = services.HasService< replay::Service >( );
}

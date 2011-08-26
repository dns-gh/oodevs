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
#include "gaming/Services.h"
#include "gaming/Simulation.h"
#include "protocol/ReplaySenders.h"

// -----------------------------------------------------------------------------
// Name: IndicatorPlotFactory constructor
// Created: SBO 2009-04-30
// -----------------------------------------------------------------------------
IndicatorPlotFactory::IndicatorPlotFactory( QMainWindow* mainWindow, kernel::Controllers& controllers, Publisher_ABC& publisher, IndicatorExportDialog& exportDialog, const Simulation& simulation )
    : mainWindow_  ( mainWindow )
    , controllers_ ( controllers )
    , publisher_   ( publisher )
    , exportDialog_( exportDialog )
    , hasReplay_   ( false )
    , simulation_  ( simulation )
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
    QDockWidget* dock = new QDockWidget( "indicatorplot", mainWindow_, Qt::WDestructiveClose );
    Q3VBox* vbox = new Q3VBox( dock );
    Q3VBox* box = new Q3VBox( vbox );
    IndicatorPlot* plot = new IndicatorPlot( box, controllers_, publisher_, dock, exportDialog_, hasReplay_, request, simulation_.GetCurrentTick(), vbox );
    dock->setWidget( vbox );
    dock->setFeatures( QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable );
    vbox->show();
    dock->setProperty( "notAppropriate", QVariant( true ) );
    return plot;
}

// -----------------------------------------------------------------------------
// Name: IndicatorPlotFactory::NotifyUpdated
// Created: SBO 2009-04-30
// -----------------------------------------------------------------------------
void IndicatorPlotFactory::NotifyUpdated( const Services& services )
{
    hasReplay_ = services.HasService< replay::Service >();
}

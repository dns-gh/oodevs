// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "AfterAction.h"
#include "AfterActionFunctionList.h"
#include "AfterActionRequestList.h"
#include "clients_kernel/Controllers.h"
#include "gaming/AfterActionModel.h"
#include "gaming/Services.h"
#include "gaming/Tools.h"
#include "game_asn/AarSenders.h"
#include "icons.h"

using namespace gui;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AfterAction constructor
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
AfterAction::AfterAction( QMainWindow* window, Controllers& controllers, ItemFactory_ABC& factory, AfterActionModel& model,
                          ParametersLayer& layer, const StaticModel& staticModel, IndicatorPlotFactory& plotFactory )
    : window_      ( window )
    , controllers_ ( controllers )
    , model_       ( model )
    , functionsTab_( 0 )
    , aar_         ( false )
{
    CreateAfterActionDock( window, controllers, factory, layer, staticModel, plotFactory );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: AfterAction destructor
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
AfterAction::~AfterAction()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AfterAction::CreateAfterActionDock
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
void AfterAction::CreateAfterActionDock( QMainWindow* window, Controllers& controllers, ItemFactory_ABC& factory, ParametersLayer& layer, const StaticModel& staticModel, IndicatorPlotFactory& plotFactory )
{
    aarDock_ = new QDockWindow( window, "aar" );
    QVBox* box = new QVBox( aarDock_ );
    box->setMinimumSize( 250, 200 );
    functionsTab_ = new QTabWidget( box );

    AfterActionFunctionList* list = new AfterActionFunctionList( functionsTab_, controllers, factory, model_, layer, staticModel );
    functionsTab_->addTab( list, tools::translate( "AfterAction", "Functions" ) );

    AfterActionRequestList* requests = new AfterActionRequestList( functionsTab_, controllers, factory, plotFactory );
    functionsTab_->addTab( requests, tools::translate( "AfterAction", "Requests" ) );

    aarDock_->setResizeEnabled( true );
    aarDock_->setWidget( box );
    aarDock_->setCloseMode( QDockWindow::Always );
    aarDock_->setCaption( tools::translate( "AfterAction", "After action review" ) );
    aarDock_->hide();
    window_->setAppropriate( aarDock_, false );
    window_->setDockEnabled( aarDock_, Qt::DockTop, false );
}

// -----------------------------------------------------------------------------
// Name: AfterAction::NotifyCreated
// Created: AGE 2007-10-10
// -----------------------------------------------------------------------------
void AfterAction::NotifyCreated( const AfterActionRequest& )
{
    functionsTab_->showPage( functionsTab_->page( 1 ) );
}

// -----------------------------------------------------------------------------
// Name: AfterAction::NotifyUpdated
// Created: AGE 2007-09-28
// -----------------------------------------------------------------------------
void AfterAction::NotifyUpdated( const Services& services )
{
    const bool isAar = services.HasService< aar::Service >();
    window_->setAppropriate( aarDock_, isAar );
    if( ! aarDock_->isVisible() )
        aarDock_->setShown( isAar && !aar_ );
    aar_ = isAar;
}

// -----------------------------------------------------------------------------
// Name: AfterAction::hide
// Created: AGE 2007-09-28
// -----------------------------------------------------------------------------
void AfterAction::hide()
{
    aarDock_->hide();
}

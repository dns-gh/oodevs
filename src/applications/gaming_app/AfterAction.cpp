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
#include "gaming/Simulation.h"
#include "icons.h"

using namespace gui;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AfterAction constructor
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
AfterAction::AfterAction( QMainWindow* window, Controllers& controllers, ItemFactory_ABC& factory, AfterActionModel& model, Publisher_ABC& publisher,
                          ParametersLayer& layer, const StaticModel& staticModel )
    : window_      ( window )
    , controllers_ ( controllers )
    , model_       ( model )
    , functionsTab_( 0 )
    , replay_      ( false )
{
    CreateAfterActionDock( window, controllers, factory, publisher, layer, staticModel );
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
void AfterAction::CreateAfterActionDock( QMainWindow* window, Controllers& controllers, ItemFactory_ABC& factory, Publisher_ABC& publisher, ParametersLayer& layer, const StaticModel& staticModel )
{
    aarDock_ = new QDockWindow( window );
    QVBox* box = new QVBox( aarDock_ );
    functionsTab_ = new QTabWidget( box );

    AfterActionFunctionList* list = new AfterActionFunctionList( functionsTab_, controllers, factory, model_, layer, staticModel );
    functionsTab_->addTab( list, tr( "Functions" ) );

    AfterActionRequestList* requests = new AfterActionRequestList( functionsTab_, window, controllers, factory, publisher );
    functionsTab_->addTab( requests, tr( "Requests" ) );

    aarDock_->setResizeEnabled( true );
    aarDock_->setWidget( box );
    aarDock_->setCloseMode( QDockWindow::Always );
    aarDock_->setCaption( tr( "After action review" ) );
    aarDock_->hide();
    window_->setAppropriate( aarDock_, false );
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
void AfterAction::NotifyUpdated( const Simulation& simu )
{
    const bool replay = simu.IsReplayer();
    window_->setAppropriate( aarDock_, replay );
    if( ! aarDock_->isVisible() )
        aarDock_->setShown( replay && !replay_ );
    replay_ = replay;
}

// -----------------------------------------------------------------------------
// Name: AfterAction::hide
// Created: AGE 2007-09-28
// -----------------------------------------------------------------------------
void AfterAction::hide()
{
    aarDock_->hide();
}

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
#include "actions_gui/InterfaceBuilder_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Tools.h"
#include "gaming/Services.h"
#include "protocol/ReplaySenders.h"
#include "protocol/AarSenders.h"

using namespace gui;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AfterAction constructor
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
AfterAction::AfterAction( QMainWindow* window, Controllers& controllers, ItemFactory_ABC& factory, AfterActionModel& model,
                          IndicatorPlotFactory& plotFactory, actions::gui::InterfaceBuilder_ABC& interfaceBuilder )
    : window_      ( window )
    , functionsTab_( 0 )
    , aar_         ( false )
    , firstUpdate_ ( true )
{
    CreateAfterActionDock( window, controllers, factory, model, plotFactory, interfaceBuilder );
    controllers.Register( *this );
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
void AfterAction::CreateAfterActionDock( QMainWindow* window, Controllers& controllers, ItemFactory_ABC& factory, AfterActionModel& model, IndicatorPlotFactory& plotFactory, actions::gui::InterfaceBuilder_ABC& interfaceBuilder )
{
    aarDock_ = new QDockWidget( "aar", window );
    aarDock_->setObjectName( tools::translate( "AfterAction", "After action review" ) );
    window->addDockWidget( Qt::LeftDockWidgetArea, aarDock_ );
    Q3VBox* box = new Q3VBox( aarDock_ );
    box->setMinimumSize( 250, 200 );
    functionsTab_ = new QTabWidget( box );

    AfterActionFunctionList* list = new AfterActionFunctionList( functionsTab_, controllers, factory, model, interfaceBuilder );
    functionsTab_->addTab( list, tools::translate( "AfterAction", "Functions" ) );

    AfterActionRequestList* requests = new AfterActionRequestList( functionsTab_, controllers, factory, plotFactory );
    functionsTab_->addTab( requests, tools::translate( "AfterAction", "Requests" ) );

    aarDock_->setFeatures( QDockWidget::AllDockWidgetFeatures );
    aarDock_->setWidget( box );
    aarDock_->setCaption( tools::translate( "AfterAction", "After action review" ) );
    aarDock_->hide();
    setProperty( "notAppropriate", QVariant() );
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
    if( !firstUpdate_ )
        aar_ = aarDock_->isShown();
    firstUpdate_ = false;
    const bool isAar = services.HasService< aar::Service >() && services.HasService< replay::Service >();
    aarDock_->setShown( aar_ && isAar );
    if( isAar )
        setProperty( "notAppropriate", QVariant() );
    else
        setProperty( "notAppropriate", QVariant( true ) );
}

// -----------------------------------------------------------------------------
// Name: AfterAction::SetStartup
// Created: FPO 2011-03-17
// -----------------------------------------------------------------------------
void AfterAction::SetStartup()
{
    aar_ = aarDock_->isShown();
    aarDock_->hide();
}

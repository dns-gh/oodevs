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
AfterAction::AfterAction( QWidget* parent, Controllers& controllers, AfterActionModel& model,
                          IndicatorPlotFactory& plotFactory, actions::gui::InterfaceBuilder_ABC& interfaceBuilder )
    : gui::RichDockWidget( controllers, parent, "after-action" )
    , functionsTab_( 0 )
    , firstUpdate_ ( true )
{
    Q3VBox* box = new Q3VBox( this );
    box->setMinimumSize( 250, 200 );
    functionsTab_ = new QTabWidget( box );

    AfterActionFunctionList* list = new AfterActionFunctionList( functionsTab_, controllers, model, interfaceBuilder );
    functionsTab_->addTab( list, tools::translate( "AfterAction", "Functions" ) );

    AfterActionRequestList* requests = new AfterActionRequestList( functionsTab_, controllers, plotFactory );
    functionsTab_->addTab( requests, tools::translate( "AfterAction", "Requests" ) );

    setFeatures( QDockWidget::AllDockWidgetFeatures );
    setWidget( box );
    setWindowTitle( tools::translate( "AfterAction", "After action review" ) );
    setProperty( "notAppropriate", QVariant() );
    hide();

    controllers_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: AfterAction destructor
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
AfterAction::~AfterAction()
{
    controllers_.Unregister( *this );
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
    const bool isAar = services.HasService< aar::Service >() && services.HasService< replay::Service >();
    setVisible( !firstUpdate_ && isShown() && isAar );
    if( firstUpdate_ )
        firstUpdate_ = false;
    if( isAar )
        setProperty( "notAppropriate", QVariant() );
    else
        setProperty( "notAppropriate", QVariant( true ) );
}

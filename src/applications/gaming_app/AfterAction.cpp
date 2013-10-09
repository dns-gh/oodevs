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
#include "clients_kernel/Tools.h"

// -----------------------------------------------------------------------------
// Name: AfterAction constructor
// Created: AGE 2007-09-17
// -----------------------------------------------------------------------------
AfterAction::AfterAction( QWidget* parent, kernel::Controllers& controllers, AfterActionModel& model,
                          IndicatorPlotFactory& plotFactory, actions::gui::InterfaceBuilder_ABC& interfaceBuilder )
    : gui::RichDockWidget( controllers, parent, "after-action" )
    , functionsTab_( 0 )
{
    QWidget* main = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout( main );
    functionsTab_ = new QTabWidget;

    AfterActionFunctionList* list = new AfterActionFunctionList( functionsTab_, controllers, model, interfaceBuilder );
    functionsTab_->addTab( list, tools::translate( "AfterAction", "Functions" ) );

    AfterActionRequestList* requests = new AfterActionRequestList( functionsTab_, controllers, plotFactory );
    functionsTab_->addTab( requests, tools::translate( "AfterAction", "Requests" ) );

    setFeatures( QDockWidget::AllDockWidgetFeatures );
    setWindowTitle( tools::translate( "AfterAction", "After action review" ) );
    layout->addWidget( functionsTab_ );
    setWidget( main );
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

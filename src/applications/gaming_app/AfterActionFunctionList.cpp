// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "AfterActionFunctionList.h"
#include "clients_kernel/Controllers.h"
#include "clients_gui/ValuedListItem.h"
#include "gaming/AfterActionModel.h"
#include "gaming/AfterActionFunction.h"
#include <qtoolbox.h>

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: AfterActionFunctionList constructor
// Created: AGE 2007-09-21
// -----------------------------------------------------------------------------
AfterActionFunctionList::AfterActionFunctionList( QWidget* parent, Controllers& controllers, gui::ItemFactory_ABC& factory )
    : QVBox( parent )
    , controllers_( controllers )
{
    buttons_ = new QToolBox( this );
    functions_ = new ListDisplayer< AfterActionFunctionList >( this, *this, factory );
    functions_->AddColumn( tr( "Name" ) );
    parameters_ = new QGroupBox( tr( "Parameters" ), this );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: AfterActionFunctionList destructor
// Created: AGE 2007-09-21
// -----------------------------------------------------------------------------
AfterActionFunctionList::~AfterActionFunctionList()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: AfterActionFunctionList::NotifyUpdated
// Created: AGE 2007-09-21
// -----------------------------------------------------------------------------
void AfterActionFunctionList::NotifyUpdated( const AfterActionModel& model )
{
    functions_->DeleteTail( 
        functions_->DisplayList( model.CreateFunctionIterator() )
        );
}

// -----------------------------------------------------------------------------
// Name: AfterActionFunctionList::Display
// Created: AGE 2007-09-21
// -----------------------------------------------------------------------------
void AfterActionFunctionList::Display( const AfterActionFunction& function, Displayer_ABC& , ValuedListItem* item )
{
    item->SetNamed( function );
}


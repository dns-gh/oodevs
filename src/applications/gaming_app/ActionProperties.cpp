// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ActionProperties.h"
#include "ActionsListView.h"
#include "SummariesDisplayer.h"
#include "actions/Action_ABC.h"
#include "actions/ActionTiming.h"
#include "clients_kernel/Displayable_ABC.h"
#include "clients_kernel/Controllers.h"

// -----------------------------------------------------------------------------
// Name: ActionProperties constructor
// Created: SBO 2010-05-04
// -----------------------------------------------------------------------------
ActionProperties::ActionProperties( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory )
    : QSplitter( Qt::Vertical, parent )
    , controllers_( controllers )
    , selected_( controllers )
{
    Q3GroupBox* group = new Q3GroupBox( 2, Qt::Horizontal, tr( "Properties" ), this );
    display_.reset( new SummariesDisplayer( group ) );
    new ActionsListView( this, controllers, factory );
    controllers_.Register( *this );
    hide();
}

// -----------------------------------------------------------------------------
// Name: ActionProperties destructor
// Created: SBO 2010-05-04
// -----------------------------------------------------------------------------
ActionProperties::~ActionProperties()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ActionProperties::NotifySelected
// Created: SBO 2010-05-04
// -----------------------------------------------------------------------------
void ActionProperties::NotifySelected( const actions::Action_ABC* action )
{
    if( selected_ != action )
    {
        selected_ = action;
        setShown( selected_ != 0 );
        if( selected_ )
            selected_->Interface().Apply( &kernel::Displayable_ABC::DisplayInSummary, *display_ );
    }
}

// -----------------------------------------------------------------------------
// Name: ActionProperties::NotifyUpdated
// Created: SBO 2010-05-04
// -----------------------------------------------------------------------------
void ActionProperties::NotifyUpdated( const actions::ActionTiming& timing )
{
    if( selected_ && selected_->Retrieve< actions::ActionTiming >() == &timing )
    {
        const actions::Action_ABC* selected = selected_;
        selected_ = 0;
        NotifySelected( selected );
    }
}

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ActionsListView.h"
#include "icons.h"
#include "actions/Action_ABC.h"
#include "actions/Parameter_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_gui/Tools.h"

// -----------------------------------------------------------------------------
// Name: ActionsListView constructor
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
ActionsListView::ActionsListView( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory )
    : ActionsListViewBase( parent, *this, factory, "ActionsListView" )
    , controllers_( controllers )
    , parameter_  ( MAKE_PIXMAP( parameter2 ) )
{
    AddColumn( tools::translate( "Parameter", "Parameter" ) );
    AddColumn( tools::translate( "Parameter", "Value" ) );
    setResizeMode( Q3ListView::LastColumn );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ActionsListView destructor
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
ActionsListView::~ActionsListView()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ActionsListView::NotifyUpdated
// Created: SBO 2007-03-19
// -----------------------------------------------------------------------------
void ActionsListView::NotifyUpdated( const actions::Action_ABC& action )
{
    DeleteTail( DisplayList( action.CreateIterator() ) );
}

// -----------------------------------------------------------------------------
// Name: ActionsListView::NotifySelected
// Created: SBO 2010-05-03
// -----------------------------------------------------------------------------
void ActionsListView::NotifySelected( const actions::Action_ABC* action )
{
    if( action )
        NotifyUpdated( *action );
    else
        setSelected( currentItem(), false );
}

// -----------------------------------------------------------------------------
// Name: ActionsListView::Display
// Created: SBO 2007-03-19
// -----------------------------------------------------------------------------
void ActionsListView::Display( const actions::Parameter_ABC& param, kernel::Displayer_ABC& displayer, gui::ValuedListItem* item )
{
    item->setPixmap( 0, parameter_ );
    param.Display( displayer );
    DeleteTail( DisplayList( param.CreateIterator(), item ) );
}

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
#include "clients_kernel/Controllers.h"
#include "gaming/Action_ABC.h"
#include "gaming/ActionParameter_ABC.h"
#include "icons.h"

// -----------------------------------------------------------------------------
// Name: ActionsListView constructor
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
ActionsListView::ActionsListView( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory )
    : gui::ListDisplayer< ActionsListView >( parent, *this, factory )
    , controllers_( controllers )
    , factory_( factory )
{
    AddColumn( "" )
    .AddColumn( tr( "Action" ) )
    .AddColumn( tr( "Value" ) );
    header()->setLabel( 0, MAKE_PIXMAP( check ), "", 25 );
    setColumnWidth( 1, 80 );

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ActionsListView destructor
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
ActionsListView::~ActionsListView()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: ActionsListView::NotifyCreated
// Created: SBO 2007-03-19
// -----------------------------------------------------------------------------
void ActionsListView::NotifyCreated( const Action_ABC& action )
{
    gui::ValuedListItem* item = factory_.CreateItem( this, firstChild() );
    item->SetValue( &action );
    item->setPixmap( 0, MAKE_PIXMAP( check ) );
    item->setText( 1, action.GetName() );
    item->setText( 2, tr( "Target: %1" ).arg( action.GetEntity().GetName() ) );
}

// -----------------------------------------------------------------------------
// Name: ActionsListView::NotifyUpdated
// Created: SBO 2007-03-19
// -----------------------------------------------------------------------------
void ActionsListView::NotifyUpdated( const Action_ABC& action )
{
    if( gui::ValuedListItem* item = gui::FindItem( &action, firstChild() ) )
        DeleteTail( DisplayList( action.CreateIterator(), item ) );
}

// -----------------------------------------------------------------------------
// Name: ActionsListView::NotifyDeleted
// Created: SBO 2007-03-19
// -----------------------------------------------------------------------------
void ActionsListView::NotifyDeleted( const Action_ABC& action )
{
    if( gui::ValuedListItem* item = gui::FindItem( &action, firstChild() ) )
        RemoveItem( item );
}

// -----------------------------------------------------------------------------
// Name: ActionsListView::Display
// Created: SBO 2007-03-19
// -----------------------------------------------------------------------------
void ActionsListView::Display( const ActionParameter_ABC& param, kernel::Displayer_ABC& displayer, gui::ValuedListItem* item )
{
    param.Display( displayer );
}

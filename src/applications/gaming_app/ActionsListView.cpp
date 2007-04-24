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
#include "GamingListItemDisplayer.h"
#include "ActionsToolbar.h"
#include "clients_kernel/Controllers.h"
#include "gaming/Action_ABC.h"
#include "gaming/ActionParameter_ABC.h"
#include "icons.h"

// -----------------------------------------------------------------------------
// Name: ActionsListView constructor
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
ActionsListView::ActionsListView( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, const ActionsToolbar& recorder )
    : gui::ListView< ActionsListView >( parent, *this, factory )
    , controllers_( controllers )
    , factory_( factory )
    , recorder_( recorder )
    , mission_  ( MAKE_PIXMAP( mission ) )
    , parameter_( MAKE_PIXMAP( parameter ) )
{
    sub_ = new GamingListItemDisplayer();
    AddColumn( tr( "Action" ) );
    AddColumn( tr( "Value" ) );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ActionsListView destructor
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
ActionsListView::~ActionsListView()
{
    controllers_.Remove( *this );
    delete sub_;
}

// -----------------------------------------------------------------------------
// Name: ActionsListView::AddColumn
// Created: SBO 2007-03-28
// -----------------------------------------------------------------------------
void ActionsListView::AddColumn( const QString& column )
{
    addColumn( column );
    sub_->AddColumn( column );
}

// -----------------------------------------------------------------------------
// Name: ActionsListView::NotifyCreated
// Created: SBO 2007-03-19
// -----------------------------------------------------------------------------
void ActionsListView::NotifyCreated( const Action_ABC& action )
{
    if( !recorder_.IsRecording() )
        return;
    gui::ValuedListItem* item = factory_.CreateItem( this, firstChild() );
    item->SetValue( &action );
    item->setPixmap( 0, mission_ );
    item->setText( 0, action.GetName() );
    item->setText( 1, tr( "Target: %1" ).arg( action.GetEntity().GetName() ) );
}

// -----------------------------------------------------------------------------
// Name: ActionsListView::NotifyUpdated
// Created: SBO 2007-03-19
// -----------------------------------------------------------------------------
void ActionsListView::NotifyUpdated( const Action_ABC& action )
{
    if( gui::ValuedListItem* item = gui::FindItem( &action, firstChild() ) )
        DeleteTail( gui::ListView< ActionsListView >::Display( action.CreateIterator(), item ) );
}

// -----------------------------------------------------------------------------
// Name: ActionsListView::NotifyDeleted
// Created: SBO 2007-03-19
// -----------------------------------------------------------------------------
void ActionsListView::NotifyDeleted( const Action_ABC& action )
{
    delete gui::FindItem( &action, firstChild() );
}

// -----------------------------------------------------------------------------
// Name: ActionsListView::Display
// Created: SBO 2007-03-19
// -----------------------------------------------------------------------------
void ActionsListView::Display( const ActionParameter_ABC& param, gui::ValuedListItem* item )
{
    item->setPixmap( 0, parameter_ );
    param.Display( (*sub_)( item ) );
    DeleteTail( gui::ListView< ActionsListView >::Display( param.CreateIterator(), item ) );
}

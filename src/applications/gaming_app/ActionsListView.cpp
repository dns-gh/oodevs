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
#include "moc_ActionsListView.cpp"
#include "GamingListItemDisplayer.h"
#include "clients_kernel/Controllers.h"
#include "gaming/Action_ABC.h"
#include "gaming/ActionParameter_ABC.h"
#include "gaming/ActionTiming.h"
#include "icons.h"

// -----------------------------------------------------------------------------
// Name: ActionsListView constructor
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
ActionsListView::ActionsListView( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory )
    : ActionsListViewBase( parent, *this, factory )
    , controllers_( controllers )
    , factory_    ( factory )
    , mission_    ( MAKE_PIXMAP( mission ) )
    , checkboxOn_ ( MAKE_PIXMAP( checkbox_on ) )
    , checkboxOff_( MAKE_PIXMAP( checkbox_off ) )
    , parameter_  ( MAKE_PIXMAP( parameter2 ) )
{
    sub_ = new GamingListItemDisplayer();
    AddColumn( tr( "S" ), Qt::AlignHCenter, 20 );
    AddColumn( tr( "Time" ), Qt::AlignHCenter, 60 );
    AddColumn( tr( "Action" ) );
    AddColumn( tr( "Value" ) );
    setSortColumn( 1 );
    setResizeMode( QListView::LastColumn );

    connect( this, SIGNAL( clicked( QListViewItem*, const QPoint&, int ) ), SLOT( OnItemClicked( QListViewItem*, const QPoint&, int ) ) );

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ActionsListView destructor
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
ActionsListView::~ActionsListView()
{
    controllers_.Unregister( *this );
    delete sub_;
}

// -----------------------------------------------------------------------------
// Name: ActionsListView::AddColumn
// Created: SBO 2007-03-28
// -----------------------------------------------------------------------------
void ActionsListView::AddColumn( const QString& column, int alignment /*=AlignAuto*/, int size /*= -1*/ )
{
    int col = addColumn( column, size );
    setColumnAlignment( col, alignment );
    sub_->AddColumn( column );
}

// -----------------------------------------------------------------------------
// Name: ActionsListView::NotifyCreated
// Created: SBO 2007-03-19
// -----------------------------------------------------------------------------
void ActionsListView::NotifyCreated( const Action_ABC& action )
{
    gui::ValuedListItem* item = factory_.CreateItem( this, firstChild() );
    item->SetValue( &action );
    if( const ActionTiming* timing = action.Retrieve< ActionTiming >() )
    {
        item->setPixmap( 0, timing->IsEnabled() ? checkboxOn_ : checkboxOff_ );
        item->setText( 1, timing->GetTime().toString( tr( "MM-dd hh:mm" ) ) );
    }
    item->setPixmap( 2, mission_ );
    item->setText( 2, action.GetName() );
    item->setText( 3, tr( "Target: %1" ).arg( action.GetEntity().GetName() ) );
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
// Name: ActionsListView::NotifyUpdated
// Created: SBO 2007-06-28
// -----------------------------------------------------------------------------
void ActionsListView::NotifyUpdated( const ActionTiming& extension )
{
    if( QListViewItem* item = gui::FindItem( &extension.GetAction(), firstChild() ) )
    {
        item->setPixmap( 0, extension.IsEnabled() ? checkboxOn_ : checkboxOff_ );
        item->setText  ( 1, extension.GetTime().toString( tr( "MM-d hh:mm" ) ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ActionsListView::Display
// Created: SBO 2007-03-19
// -----------------------------------------------------------------------------
void ActionsListView::Display( const ActionParameter_ABC& param, gui::ValuedListItem* item )
{
    item->setPixmap( 2, parameter_ );
    param.Display( (*sub_)( item ) );
    DeleteTail( gui::ListView< ActionsListView >::Display( param.CreateIterator(), item ) );
}

// -----------------------------------------------------------------------------
// Name: ActionsListView::OnItemClicked
// Created: SBO 2007-06-27
// -----------------------------------------------------------------------------
void ActionsListView::OnItemClicked( QListViewItem* item, const QPoint&, int col )
{
    if( col != 0 || !item )
        return;
    gui::ValuedListItem* value = static_cast< gui::ValuedListItem* >( item );
    if( value->IsA< const Action_ABC >() )
    {
        const Action_ABC* action = value->GetValue< const Action_ABC >();
        if( ActionTiming* timing = const_cast< ActionTiming* >( action->Retrieve< ActionTiming >() ) )
            timing->ToggleEnabled();
    }
}

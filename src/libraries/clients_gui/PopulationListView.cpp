// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "PopulationListView.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/OptionVariant.h"
#include "ItemFactory_ABC.h"
#include "ValuedListItem.h"

#include "moc_PopulationListView.cpp"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: PopulationListView constructor
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
PopulationListView::PopulationListView( QWidget* pParent, Controllers& controllers, ItemFactory_ABC& factory )
    : QListView   ( pParent )
    , controllers_( controllers )
    , factory_( factory )
    , pPopupMenu_ ( 0 )
    , currentTeam_( 0 )
{
    setMinimumSize( 1, 1 );
    addColumn( tr( "Populations" ) );
    setRootIsDecorated( true );
    setResizeMode( QListView::LastColumn );
    setAcceptDrops( true );
    header()->hide();

    connect( this, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnContextMenuRequested( QListViewItem*, const QPoint&, int ) ) );
    connect( this, SIGNAL( doubleClicked       ( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnRequestCenter() ) );
    connect( this, SIGNAL( selectionChanged    ( QListViewItem* ) ),                     this, SLOT( OnSelectionChange( QListViewItem* ) ) );
    connect( this, SIGNAL( spacePressed        ( QListViewItem* ) ),                     this, SLOT( OnRequestCenter() ) );

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: PopulationListView destructor
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
PopulationListView::~PopulationListView()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: PopulationListView::NotifyCreated
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void PopulationListView::NotifyCreated( const Population_ABC& popu )
{
    const Team_ABC& team = popu.GetTeam();
    ValuedListItem* teamItem = FindSibling( &team, firstChild() );
    if( ! teamItem )
    {
        teamItem = factory_.CreateItem( this );
        teamItem->Set( &team, team.GetName().c_str() );
    }
    factory_.CreateItem( teamItem )->Set( &popu, popu.GetName().c_str() );
}

// -----------------------------------------------------------------------------
// Name: PopulationListView::NotifyDeleted
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void PopulationListView::NotifyDeleted( const Population_ABC& popu )
{
    QListViewItem* item = FindItem( &popu, firstChild() );
    QListViewItem* teamItem = item ? item->parent() : 0;
    delete item;
    if( teamItem && ! teamItem->childCount() )
        delete teamItem;
}

// $$$$ AGE 2006-03-22: somehow factor these things
// -----------------------------------------------------------------------------
// Name: PopulationListView::OnSelectionChange
// Created: AGE 2006-02-17
// -----------------------------------------------------------------------------
void PopulationListView::OnSelectionChange( QListViewItem* i )
{
    if( i )
    {
        ValuedListItem* item = (ValuedListItem*)( i );
        item->Select( controllers_.actions_ );
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationListView::OnRequestCenter
// Created: AGE 2006-03-22
// -----------------------------------------------------------------------------
void PopulationListView::OnRequestCenter()
{
    if( selectedItem() )
    {
        ValuedListItem* item = (ValuedListItem*)( selectedItem() );
        item->Activate( controllers_.actions_ );
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationListView::OnContextMenuRequested
// Created: AGE 2006-07-04
// -----------------------------------------------------------------------------
void PopulationListView::OnContextMenuRequested( QListViewItem* i, const QPoint& pos, int )
{
    if( i )
    {
        ValuedListItem* item = (ValuedListItem*)( i );
        item->ContextMenu( controllers_.actions_, pos );
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationListView::Select
// Created: AGE 2006-03-21
// -----------------------------------------------------------------------------
void PopulationListView::Select( const Population_ABC& popu )
{
    setSelected( FindItem( &popu, firstChild() ), true );
    ensureItemVisible( selectedItem() );
}

// -----------------------------------------------------------------------------
// Name: PopulationListView::OptionChanged
// Created: AGE 2006-03-27
// -----------------------------------------------------------------------------
void PopulationListView::OptionChanged( const std::string& name, const OptionVariant& value )
{
    if( name == "CurrentTeam" )
        currentTeam_ = value.To< const Team_ABC* >();
    ValuedListItem* item = (ValuedListItem*)( firstChild() );
    while( item )
    {
        item->setVisible( ! currentTeam_ || item->Holds( currentTeam_ ) );
        item = (ValuedListItem*)( item->nextSibling() );
    }
}

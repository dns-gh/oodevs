// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

/* TRANSLATOR gui::InhabitantListView */

#include "clients_gui_pch.h"
#include "InhabitantListView.h"
#include "clients_kernel/TacticalHierarchies.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: InhabitantListView constructor
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
InhabitantListView::InhabitantListView( QWidget* pParent, Controllers& controllers, ItemFactory_ABC& factory, const Profile_ABC& profile )
    : EntityListView( pParent, controllers, factory, profile )
    , controllers_( controllers )
{
    addColumn( tr( "Populations" ) );
    setAcceptDrops( true );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: InhabitantListView destructor
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
InhabitantListView::~InhabitantListView()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: InhabitantListView::NotifyCreated
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
void InhabitantListView::NotifyCreated( const Inhabitant_ABC& inhabitant )
{
    const Entity_ABC& team = inhabitant.Get< TacticalHierarchies >().GetTop();
    ValuedListItem* teamItem = FindSibling( &team, firstChild() );
    if( ! teamItem )
    {
        teamItem = CreateItem( this );
        teamItem->SetNamed( team );
    }
    ValuedListItem* popItem = CreateItem( teamItem );
    popItem->SetNamed( static_cast< const Entity_ABC& >( inhabitant ) );
    popItem->setDragEnabled( true );
    popItem->SetToolTip( QString( "%1 [%2]" ).arg( inhabitant.GetName() ).arg( inhabitant.GetId() ) );
}

// -----------------------------------------------------------------------------
// Name: InhabitantListView::NotifyDeleted
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
void InhabitantListView::NotifyDeleted( const Inhabitant_ABC& inhabitant )
{
    delete FindItem( static_cast< const Entity_ABC* >( &inhabitant ), firstChild() );
}

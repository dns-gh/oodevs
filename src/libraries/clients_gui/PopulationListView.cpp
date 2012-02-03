// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

/* TRANSLATOR gui::PopulationListView */

#include "clients_gui_pch.h"
#include "PopulationListView.h"
#include "clients_kernel/TacticalHierarchies.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: PopulationListView constructor
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
PopulationListView::PopulationListView( QWidget* pParent, Controllers& controllers, ItemFactory_ABC& factory, const Profile_ABC& profile )
    : EntityListView( pParent, controllers, factory, profile )
    , controllers_( controllers )
{
    addColumn( tr( "Crowds" ) );
    setAcceptDrops( true );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: PopulationListView destructor
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
PopulationListView::~PopulationListView()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: PopulationListView::NotifyCreated
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void PopulationListView::NotifyCreated( const Population_ABC& popu )
{
    const Entity_ABC& team = popu.Get< TacticalHierarchies >().GetTop();
    ValuedListItem* teamItem = FindSibling( &team, firstChild() );
    if( ! teamItem )
    {
        teamItem = CreateItem( this );
        teamItem->SetNamed( team );
    }
    ValuedListItem* popItem = CreateItem( teamItem );
    popItem->SetNamed( static_cast< const Entity_ABC& >( popu ) );
    popItem->setDragEnabled( true );
    popItem->SetToolTip( QString( "%1 [%2]" ).arg( popu.GetName() ).arg( popu.GetId() ) );
}

// -----------------------------------------------------------------------------
// Name: PopulationListView::NotifyDeleted
// Created: SBO 2006-10-30
// -----------------------------------------------------------------------------
void PopulationListView::NotifyDeleted( const Population_ABC& element )
{
    delete FindItem( static_cast< const Entity_ABC* >( &element ), firstChild() );
}

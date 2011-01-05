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
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/TacticalHierarchies.h"

#pragma warning( disable : 4355 )

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: PopulationListView constructor
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
PopulationListView::PopulationListView( QWidget* pParent, Controllers& controllers, ItemFactory_ABC& factory, const kernel::Profile_ABC& profile )
    : EntityListView( pParent, controllers, factory, profile )
    , controllers_( controllers )
    , factory_    ( factory )
{
    addColumn( tr( "Populations" ) );
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
        teamItem = factory_.CreateItem( this );
        teamItem->SetNamed( team );
    }
    ValuedListItem* popItem = factory_.CreateItem( teamItem );
    popItem->SetNamed( (const Entity_ABC&)popu );
    popItem->setDragEnabled( true );
}

// -----------------------------------------------------------------------------
// Name: PopulationListView::NotifyDeleted
// Created: SBO 2006-10-30
// -----------------------------------------------------------------------------
void PopulationListView::NotifyDeleted( const Population_ABC& element )
{
    delete FindItem( (const Entity_ABC*)&element, firstChild() );
}

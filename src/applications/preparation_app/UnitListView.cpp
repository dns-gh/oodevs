// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "UnitListView.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/AutomatType.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: UnitListView constructor
// Created: SBO 2006-08-28
// -----------------------------------------------------------------------------
UnitListView::UnitListView( QWidget* parent, Controllers& controllers, AgentTypes& list, ItemFactory_ABC& factory )
    : ListView< UnitListView >( parent, *this, factory )
    , controllers_( controllers )
    , types_( list )
{
    setMinimumSize( 1, 1 );
    addColumn( tr( "Type d'unité" ) );
    setRootIsDecorated( true );
    header()->hide();

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: UnitListView destructor
// Created: SBO 2006-08-28
// -----------------------------------------------------------------------------
UnitListView::~UnitListView()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: UnitListView::SortByNature
// Created: SBO 2006-08-28
// -----------------------------------------------------------------------------
void UnitListView::SortByNature( const std::string& nature )
{
    // $$$$ SBO 2006-08-28: TODO
}

// -----------------------------------------------------------------------------
// Name: UnitListView::NotifyUpdated
// Created: SBO 2006-08-28
// -----------------------------------------------------------------------------
void UnitListView::NotifyUpdated( const ModelLoaded& )
{
    Iterator< const AutomatType& > it( types_.Resolver< AutomatType >::CreateIterator() );
    DeleteTail( ListView< UnitListView >::Display( it, this ) );
}

// -----------------------------------------------------------------------------
// Name: UnitListView::Display
// Created: SBO 2006-08-28
// -----------------------------------------------------------------------------
void UnitListView::Display( const AgentType& type, ValuedListItem* item )
{
    item->SetNamed( type );
}

// -----------------------------------------------------------------------------
// Name: UnitListView::Display
// Created: SBO 2006-08-28
// -----------------------------------------------------------------------------
void UnitListView::Display( const AutomatType& type, ValuedListItem* item )
{
    item->SetNamed( type );
    Iterator< const AgentType& > it( type.CreateIterator() );
    DeleteTail( ListView< UnitListView >::Display( it, item ) );

    const AgentType& pc = *type.GetTypePC();
    ValuedListItem* pcItem = new ValuedListItem( item );
    pcItem->SetNamed( pc );
}

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

/* TRANSLATOR gui::PopulationTypesListView */

#include "clients_gui_pch.h"
#include "PopulationTypesListView.h"
#include "moc_PopulationTypesListView.cpp"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/PopulationType.h"
#include "Tools.h"

#pragma warning( disable : 4355 ) // $$$$ SBO 2008-05-14: 'this' : used in base member initializer list

using namespace gui;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: PopulationTypesListView constructor
// Created: SBO 2006-11-09
// -----------------------------------------------------------------------------
PopulationTypesListView::PopulationTypesListView( QWidget* parent, kernel::Controllers& controllers, const tools::Resolver_ABC< kernel::PopulationType >& types, gui::ItemFactory_ABC& factory )
    : gui::ListView< PopulationTypesListView >( parent, *this, factory )
    , controllers_( controllers )
    , types_( types )
{
    setResizeMode( Q3ListView::AllColumns );
    setMinimumSize( 1, 1 );
    addColumn( tools::translate( "gui::PopulationTypesListView", "Crowd type" ) );
    setRootIsDecorated( true );
    header()->hide();
    setSorting( -1 );

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: PopulationTypesListView destructor
// Created: SBO 2006-11-09
// -----------------------------------------------------------------------------
PopulationTypesListView::~PopulationTypesListView()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: PopulationTypesListView::Display
// Created: SBO 2006-11-09
// -----------------------------------------------------------------------------
void PopulationTypesListView::Display( const kernel::PopulationType& type, gui::ValuedListItem* item )
{
    item->setDragEnabled( true );
    item->SetNamed( type );
}

// -----------------------------------------------------------------------------
// Name: PopulationTypesListView::NotifyUpdated
// Created: SBO 2006-11-09
// -----------------------------------------------------------------------------
void PopulationTypesListView::NotifyUpdated( const kernel::ModelLoaded& )
{
    tools::Iterator< const PopulationType& > it( types_.CreateIterator() );
    DeleteTail( gui::ListView< PopulationTypesListView >::Display( it, this ) );
}

// -----------------------------------------------------------------------------
// Name: PopulationTypesListView::NotifyUpdated
// Created: SBO 2006-11-09
// -----------------------------------------------------------------------------
void PopulationTypesListView::NotifyUpdated( const kernel::ModelUnLoaded& )
{
    clear();
}

// -----------------------------------------------------------------------------
// Name: PopulationTypesListView::startDrag
// Created: SBO 2006-11-09
// -----------------------------------------------------------------------------
void PopulationTypesListView::startDrag()
{
    gui::ValuedListItem* pItem = static_cast< gui::ValuedListItem* >( selectedItem() );
    if( !pItem )
        return;
    const PopulationType* pType = pItem->GetValue< const PopulationType >();
    emit StartDrag( pType );
}

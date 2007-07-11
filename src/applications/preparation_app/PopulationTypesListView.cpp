// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "PopulationTypesListView.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/PopulationType.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: PopulationTypesListView constructor
// Created: SBO 2006-11-09
// -----------------------------------------------------------------------------
PopulationTypesListView::PopulationTypesListView( QWidget* parent, kernel::Controllers& controllers, const kernel::Resolver_ABC< kernel::PopulationType >& types, gui::ItemFactory_ABC& factory )
    : gui::ListView< PopulationTypesListView >( parent, *this, factory )
    , controllers_( controllers )
    , types_( types )
{
    setMinimumSize( 1, 1 );
    addColumn( tr( "Population type" ) );
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
    Iterator< const PopulationType& > it( types_.CreateIterator() );
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
// Name: PopulationTypesListView::dragObject
// Created: SBO 2006-11-09
// -----------------------------------------------------------------------------
QDragObject* PopulationTypesListView::dragObject()
{
    gui::ValuedListItem* pItem = static_cast< gui::ValuedListItem* >( selectedItem() );
    if( !pItem )
        return 0;

    QByteArray* pBytes = new QByteArray();
    const PopulationType* pType = pItem->GetValue< const PopulationType >();
    pBytes->setRawData( (const char*)( &pType ), sizeof( PopulationType* ) );
    QStoredDrag* data = new QStoredDrag( "csword/PopulationType", this );
    data->setEncodedData( *pBytes );
    return data;
    
}

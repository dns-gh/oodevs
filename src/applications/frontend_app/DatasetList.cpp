// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_app_pch.h"
#include "DatasetList.h"
#include "frontend/PhysicalModel.h"

// -----------------------------------------------------------------------------
// Name: DatasetList constructor
// Created: SBO 2007-02-01
// -----------------------------------------------------------------------------
DatasetList::DatasetList( QWidget* parent, kernel::Controllers& controllers )
    : ElementListView< frontend::Dataset >( parent, controllers )
{
    addColumn( tr( "Datasets" ) );
}

// -----------------------------------------------------------------------------
// Name: DatasetList destructor
// Created: SBO 2007-02-01
// -----------------------------------------------------------------------------
DatasetList::~DatasetList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DatasetList::NotifyCreated
// Created: SBO 2007-02-01
// -----------------------------------------------------------------------------
void DatasetList::NotifyCreated( const frontend::PhysicalModel& element )
{
    if( QListViewItem* parent = gui::FindItem( &element.GetDataset(), firstChild() ) )
    {
        gui::ValuedListItem* item = new gui::ValuedListItem( parent );
        item->SetNamed( element );
    }
}

// -----------------------------------------------------------------------------
// Name: DatasetList::NotifyUpdated
// Created: SBO 2007-02-01
// -----------------------------------------------------------------------------
void DatasetList::NotifyUpdated( const frontend::PhysicalModel& element )
{
    if( gui::ValuedListItem* item = gui::FindItem( &element, firstChild() ) )
        item->SetNamed( element );
}

// -----------------------------------------------------------------------------
// Name: DatasetList::NotifyDeleted
// Created: SBO 2007-02-01
// -----------------------------------------------------------------------------
void DatasetList::NotifyDeleted( const frontend::PhysicalModel& element )
{
    if( gui::ValuedListItem* item = gui::FindItem( &element, firstChild() ) )
        delete item;
}

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "TableItemDisplayer.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: TableItemDisplayer constructor
// Created: SBO 2006-10-18
// -----------------------------------------------------------------------------
TableItemDisplayer::TableItemDisplayer()
    : item_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TableItemDisplayer destructor
// Created: SBO 2006-10-18
// -----------------------------------------------------------------------------
TableItemDisplayer::~TableItemDisplayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TableItemDisplayer::Hide
// Created: SBO 2006-10-18
// -----------------------------------------------------------------------------
void TableItemDisplayer::Hide()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TableItemDisplayer::SubItem
// Created: SBO 2006-10-18
// -----------------------------------------------------------------------------
kernel::Displayer_ABC& TableItemDisplayer::SubItem( const QString& )
{
    NotToBeCalled( __FUNCTION__ );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: TableItemDisplayer::StartDisplay
// Created: SBO 2006-10-18
// -----------------------------------------------------------------------------
void TableItemDisplayer::StartDisplay()
{
    message_ = "";
}

// -----------------------------------------------------------------------------
// Name: TableItemDisplayer::DisplayFormatted
// Created: SBO 2006-10-18
// -----------------------------------------------------------------------------
void TableItemDisplayer::DisplayFormatted( const QString& formatted )
{
    message_ += formatted;
}

// -----------------------------------------------------------------------------
// Name: TableItemDisplayer::EndDisplay
// Created: SBO 2006-10-18
// -----------------------------------------------------------------------------
void TableItemDisplayer::EndDisplay()
{
    if( !item_ )
        throw std::runtime_error( __FUNCTION__ );
    item_->setText( message_ );
    item_ = 0;
}

// -----------------------------------------------------------------------------
// Name: TableItemDisplayer::SetItem
// Created: SBO 2006-10-18
// -----------------------------------------------------------------------------
void TableItemDisplayer::SetItem( Q3TableItem* item )
{
    item_ = item;
}

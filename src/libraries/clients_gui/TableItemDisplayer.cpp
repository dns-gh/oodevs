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
#include "PropertyTableItem.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: TableItemDisplayer constructor
// Created: SBO 2006-10-18
// -----------------------------------------------------------------------------
TableItemDisplayer::TableItemDisplayer()
    : item_( 0 )
    , color_( Qt::white )
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
    // $$$$ ABR 2012-06-04: TODO, read color and keep the remaining message
    // $$$$ ABR 2012-06-04: Should be improve, without changing color every time for every item.
    if( formatted.startsWith( "<color" ) )
    {
        xml::xistringstream xis( formatted.toStdString() );
        int red = 0;
        int green = 0;
        int blue = 0;
        xis >> xml::start( "color" )
                >> xml::attribute< int >( "red", red )
                >> xml::attribute< int >( "green", green )
                >> xml::attribute< int >( "blue", blue )
            >> xml::end;
        color_ = QColor( red, green, blue );
    }
    else
    {
        color_ = Qt::white;
        message_ += formatted;
    }
}

// -----------------------------------------------------------------------------
// Name: TableItemDisplayer::EndDisplay
// Created: SBO 2006-10-18
// -----------------------------------------------------------------------------
void TableItemDisplayer::EndDisplay()
{
    if( !item_ )
        throw std::runtime_error( __FUNCTION__ );

    static_cast< PropertyTableItem* >( item_ )->SetColor( color_ );
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

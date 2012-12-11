// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "PropertyDisplayer.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: PropertyDisplayer constructor
// Created: SBO 2006-10-18
// -----------------------------------------------------------------------------
PropertyDisplayer::PropertyDisplayer()
    : item_ ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PropertyDisplayer destructor
// Created: SBO 2006-10-18
// -----------------------------------------------------------------------------
PropertyDisplayer::~PropertyDisplayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PropertyDisplayer::Hide
// Created: SBO 2006-10-18
// -----------------------------------------------------------------------------
void PropertyDisplayer::Hide()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PropertyDisplayer::SubItem
// Created: SBO 2006-10-18
// -----------------------------------------------------------------------------
kernel::Displayer_ABC& PropertyDisplayer::SubItem( const QString& )
{
    NotToBeCalled( __FUNCTION__ );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: PropertyDisplayer::StartDisplay
// Created: SBO 2006-10-18
// -----------------------------------------------------------------------------
void PropertyDisplayer::StartDisplay()
{
    message_ = "";
}

// -----------------------------------------------------------------------------
// Name: PropertyDisplayer::DisplayFormatted
// Created: SBO 2006-10-18
// -----------------------------------------------------------------------------
void PropertyDisplayer::DisplayFormatted( const QString& formatted )
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
        color_ = QColor();
        message_ += formatted;
    }
}

// -----------------------------------------------------------------------------
// Name: PropertyDisplayer::EndDisplay
// Created: SBO 2006-10-18
// -----------------------------------------------------------------------------
void PropertyDisplayer::EndDisplay()
{
    if( !item_ )
        throw MASA_EXCEPTION( "Invalid current item." );

    item_->setData( message_, Qt::DisplayRole );
    if( color_.isValid() )
        item_->setBackground( QBrush( color_ ) );

    item_ = 0;
}

// -----------------------------------------------------------------------------
// Name: PropertyDisplayer::SetItem
// Created: LGY 2012-08-13
// -----------------------------------------------------------------------------
void PropertyDisplayer::SetItem( QStandardItem* item )
{
    item_ = item;
}

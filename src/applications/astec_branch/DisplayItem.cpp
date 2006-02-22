// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "DisplayItem.h"
#include "Agent.h"
#include "DotationType.h"
#include <qlabel.h>

// -----------------------------------------------------------------------------
// Name: DisplayItem constructor
// Created: AGE 2006-02-09
// -----------------------------------------------------------------------------
DisplayItem::DisplayItem( QWidget* parent, const char* name, bool bold )
{
    new QLabel( qApp->tr( name ), parent );
    valueLabel_ = new QLabel( parent );
    if( bold )
    {
        QFont boldFont = valueLabel_->font();
        boldFont.setBold( true );
        valueLabel_->setFont( boldFont );
    }
}
    
// -----------------------------------------------------------------------------
// Name: DisplayItem destructor
// Created: AGE 2006-02-09
// -----------------------------------------------------------------------------
DisplayItem::~DisplayItem()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DisplayItem::SubItem
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
Displayer_ABC& DisplayItem::SubItem( const char* )
{
    throw std::runtime_error( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: DisplayItem::StartDisplay
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
void DisplayItem::StartDisplay()
{
    message_ = "";
}

// -----------------------------------------------------------------------------
// Name: DisplayItem::DisplayFormatted
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
void DisplayItem::DisplayFormatted( const QString& formatted )
{
    message_ += formatted;
}

// -----------------------------------------------------------------------------
// Name: DisplayItem::EndDisplay
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
void DisplayItem::EndDisplay()
{
    valueLabel_->setText( message_ );
}

// -----------------------------------------------------------------------------
// Name: DisplayItem::Call
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
void DisplayItem::Call( const bool& value )
{
    AddToDisplay( value ? qApp->tr( "Oui" ) : qApp->tr( "Non" ) );
}

// -----------------------------------------------------------------------------
// Name: DisplayItem::Call
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
void DisplayItem::Call( const Agent& value )
{
    AddToDisplay( QString( value.GetName().c_str() ) );
    AddToDisplay( QString( " [" ) );
    AddToDisplay( QString::number( value.GetId() ) );
    AddToDisplay( QString( "]" ) );
}

// -----------------------------------------------------------------------------
// Name: DisplayItem::Call
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
void DisplayItem::Call( const DotationType& value )
{
    AddToDisplay( QString( value.GetCategory().c_str() ) );
}

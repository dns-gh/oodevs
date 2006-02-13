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
// Name: DisplayItem::Display
// Created: AGE 2006-02-09
// -----------------------------------------------------------------------------
void DisplayItem::Display( const QString& value )
{
    valueLabel_->setText( value );
}

// -----------------------------------------------------------------------------
// Name: DisplayItem::Display
// Created: AGE 2006-02-09
// -----------------------------------------------------------------------------
void DisplayItem::Display( const char* value )
{
    valueLabel_->setText( value );
}

// -----------------------------------------------------------------------------
// Name: DisplayItem::Display
// Created: AGE 2006-02-09
// -----------------------------------------------------------------------------
void DisplayItem::Display( const std::string& value )
{
    Display( value.c_str() );
}

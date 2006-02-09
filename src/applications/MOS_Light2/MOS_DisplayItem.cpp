// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "MOS_Light2_pch.h"
#include "MOS_DisplayItem.h"
#include <qlabel.h>

// -----------------------------------------------------------------------------
// Name: MOS_DisplayItem constructor
// Created: AGE 2006-02-09
// -----------------------------------------------------------------------------
MOS_DisplayItem::MOS_DisplayItem( QWidget* parent, const char* name, bool bold )
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
// Name: MOS_DisplayItem destructor
// Created: AGE 2006-02-09
// -----------------------------------------------------------------------------
MOS_DisplayItem::~MOS_DisplayItem()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MOS_DisplayItem::Display
// Created: AGE 2006-02-09
// -----------------------------------------------------------------------------
void MOS_DisplayItem::Display( const QString& value )
{
    valueLabel_->setText( value );
}

// -----------------------------------------------------------------------------
// Name: MOS_DisplayItem::Display
// Created: AGE 2006-02-09
// -----------------------------------------------------------------------------
void MOS_DisplayItem::Display( const char* value )
{
    valueLabel_->setText( value );
}

// -----------------------------------------------------------------------------
// Name: MOS_DisplayItem::Display
// Created: AGE 2006-02-09
// -----------------------------------------------------------------------------
void MOS_DisplayItem::Display( const std::string& value )
{
    Display( value.c_str() );
}

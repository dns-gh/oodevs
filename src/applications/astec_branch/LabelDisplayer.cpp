// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "LabelDisplayer.h"
#include "Agent.h"
#include "DotationType.h"
#include <qlabel.h>

// -----------------------------------------------------------------------------
// Name: LabelDisplayer constructor
// Created: AGE 2006-02-09
// -----------------------------------------------------------------------------
LabelDisplayer::LabelDisplayer( QWidget* parent, const char* name, bool bold )
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
// Name: LabelDisplayer destructor
// Created: AGE 2006-02-09
// -----------------------------------------------------------------------------
LabelDisplayer::~LabelDisplayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LabelDisplayer::SubItem
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
Displayer_ABC& LabelDisplayer::SubItem( const char* )
{
    return NotToBeCalled( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: LabelDisplayer::StartDisplay
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
void LabelDisplayer::StartDisplay()
{
    message_ = "";
}

// -----------------------------------------------------------------------------
// Name: LabelDisplayer::DisplayFormatted
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
void LabelDisplayer::DisplayFormatted( const QString& formatted )
{
    message_ += formatted;
}

// -----------------------------------------------------------------------------
// Name: LabelDisplayer::EndDisplay
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
void LabelDisplayer::EndDisplay()
{
    valueLabel_->setText( message_ );
}

// -----------------------------------------------------------------------------
// Name: LabelDisplayer::Hide
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void LabelDisplayer::Hide()
{
    // NOTHING
}

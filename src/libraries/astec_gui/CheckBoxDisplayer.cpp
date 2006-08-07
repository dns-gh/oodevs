// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_gui_pch.h"
#include "CheckBoxDisplayer.h"

// -----------------------------------------------------------------------------
// Name: CheckBoxDisplayer constructor
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
CheckBoxDisplayer::CheckBoxDisplayer( QWidget* parent, const char* name )
{
    new QLabel( qApp->tr( name ), parent );
    box_ = new QCheckBox( parent );
}

// -----------------------------------------------------------------------------
// Name: CheckBoxDisplayer destructor
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
CheckBoxDisplayer::~CheckBoxDisplayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CheckBoxDisplayer::SubItem
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
Displayer_ABC& CheckBoxDisplayer::SubItem( const char* )
{
    return NotToBeCalled( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: CheckBoxDisplayer::StartDisplay
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void CheckBoxDisplayer::StartDisplay()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CheckBoxDisplayer::DisplayFormatted
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void CheckBoxDisplayer::DisplayFormatted( const QString& )
{
    DisplayNotImplemented< QString >();
}

// -----------------------------------------------------------------------------
// Name: CheckBoxDisplayer::EndDisplay
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void CheckBoxDisplayer::EndDisplay()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CheckBoxDisplayer::Call
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void CheckBoxDisplayer::Call( const bool& value )
{
    box_->setChecked( value );
}

// -----------------------------------------------------------------------------
// Name: CheckBoxDisplayer::Hide
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void CheckBoxDisplayer::Hide()
{
    // NOTHING
}

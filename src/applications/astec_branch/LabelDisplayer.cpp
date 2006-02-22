// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
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
    throw std::runtime_error( __FUNCTION__ );
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
// Name: LabelDisplayer::Call
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
void LabelDisplayer::Call( const bool& value )
{
    AddToDisplay( value ? qApp->tr( "Oui" ) : qApp->tr( "Non" ) );
}

// -----------------------------------------------------------------------------
// Name: LabelDisplayer::Call
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
void LabelDisplayer::Call( const Agent& value )
{
    AddToDisplay( QString( value.GetName().c_str() ) );
}

// -----------------------------------------------------------------------------
// Name: LabelDisplayer::Call
// Created: AGE 2006-02-22
// -----------------------------------------------------------------------------
void LabelDisplayer::Call( const DotationType& value )
{
    AddToDisplay( QString( value.GetCategory().c_str() ) );
}

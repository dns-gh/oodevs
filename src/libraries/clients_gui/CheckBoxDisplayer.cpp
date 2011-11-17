// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "CheckBoxDisplayer.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: CheckBoxDisplayer constructor
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
CheckBoxDisplayer::CheckBoxDisplayer( QBoxLayout* parent, const QString& name )
{
    QHBoxLayout* layout = new QHBoxLayout();
    parent->addLayout( layout );
    layout->addWidget( new QLabel( name ) );
    box_ = new QCheckBox();
    layout->addWidget( box_ );
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
// Name: CheckBoxDisplayer::IsChecked
// Created: AGE 2006-09-08
// -----------------------------------------------------------------------------
bool CheckBoxDisplayer::IsChecked() const
{
    return box_->isChecked();
}

// -----------------------------------------------------------------------------
// Name: CheckBoxDisplayer::SubItem
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
Displayer_ABC& CheckBoxDisplayer::SubItem( const QString& )
{
    return NotToBeCalled( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: CheckBoxDisplayer::StartDisplay
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void CheckBoxDisplayer::StartDisplay()
{
    box_->setTristate( false );
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
// Name: CheckBoxDisplayer::Call
// Created: AGE 2006-10-17
// -----------------------------------------------------------------------------
void CheckBoxDisplayer::Call( const kernel::ValueNotSet& )
{
    box_->setTristate( true );
    box_->setNoChange();
}

// -----------------------------------------------------------------------------
// Name: CheckBoxDisplayer::Hide
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void CheckBoxDisplayer::Hide()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CheckBoxDisplayer::Clear
// Created: LDC 2009-03-18
// -----------------------------------------------------------------------------
void CheckBoxDisplayer::Clear()
{
    Call( kernel::ValueNotSet() );
}

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "SpinBoxDisplayer.h"
#include "clients_kernel/Units.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: SpinBoxDisplayer constructor
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
SpinBoxDisplayer::SpinBoxDisplayer( QBoxLayout* parent, const QString& name, int min, int max, int step )
{
    QHBoxLayout* layout = new QHBoxLayout();
    parent->addLayout( layout );
    layout->addWidget( new QLabel( name ) );
    edit_ = new QSpinBox();
    edit_->setRange( min, max );
    edit_->setSingleStep( step );
    layout->addWidget( edit_ );
}

// -----------------------------------------------------------------------------
// Name: SpinBoxDisplayer destructor
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
SpinBoxDisplayer::~SpinBoxDisplayer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SpinBoxDisplayer::GetValue
// Created: AGE 2006-09-08
// -----------------------------------------------------------------------------
int SpinBoxDisplayer::GetValue() const
{
    return edit_->value();
}

// -----------------------------------------------------------------------------
// Name: SpinBoxDisplayer::SubItem
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
Displayer_ABC& SpinBoxDisplayer::SubItem( const QString& )
{
    return NotToBeCalled( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: SpinBoxDisplayer::StartDisplay
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void SpinBoxDisplayer::StartDisplay()
{
    edit_->setSpecialValueText( QString::null );
}

// -----------------------------------------------------------------------------
// Name: SpinBoxDisplayer::DisplayFormatted
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void SpinBoxDisplayer::DisplayFormatted( const QString& )
{
    DisplayNotImplemented< QString >();
}

// -----------------------------------------------------------------------------
// Name: SpinBoxDisplayer::EndDisplay
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void SpinBoxDisplayer::EndDisplay()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SpinBoxDisplayer::Call
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void SpinBoxDisplayer::Call( const double& value )
{
    edit_->setValue( int( value ) );
}

// -----------------------------------------------------------------------------
// Name: SpinBoxDisplayer::Call
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void SpinBoxDisplayer::Call( const float& value )
{
    edit_->setValue( int( value ) );
}

// -----------------------------------------------------------------------------
// Name: SpinBoxDisplayer::Call
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void SpinBoxDisplayer::Call( const int& value )
{
    edit_->setValue( value );
}

// -----------------------------------------------------------------------------
// Name: SpinBoxDisplayer::Call
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void SpinBoxDisplayer::Call( const Unit& value )
{
    edit_->setSuffix( value.AsString() );
}

// -----------------------------------------------------------------------------
// Name: SpinBoxDisplayer::Call
// Created: AGE 2006-10-17
// -----------------------------------------------------------------------------
void SpinBoxDisplayer::Call( const kernel::ValueNotSet& )
{
    edit_->setSpecialValueText( "---" );
    edit_->setValue( edit_->minValue() );
}

// -----------------------------------------------------------------------------
// Name: SpinBoxDisplayer::Hide
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void SpinBoxDisplayer::Hide()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SpinBoxDisplayer::Clear
// Created: LDC 2009-03-18
// -----------------------------------------------------------------------------
void SpinBoxDisplayer::Clear()
{
    Call( kernel::ValueNotSet() );
}

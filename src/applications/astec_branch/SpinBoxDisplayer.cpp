// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "SpinBoxDisplayer.h"
#include "Units.h"

// -----------------------------------------------------------------------------
// Name: SpinBoxDisplayer constructor
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
SpinBoxDisplayer::SpinBoxDisplayer( QWidget* parent, const char* name, int min, int max, int step )
{
    new QLabel( qApp->tr( name ), parent );
    edit_ = new QSpinBox( min, max, step, parent );
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
// Name: SpinBoxDisplayer::SubItem
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
Displayer_ABC& SpinBoxDisplayer::SubItem( const char* )
{
    throw std::runtime_error( __FUNCTION__ ); // $$$$ AGE 2006-02-23: 
}

// -----------------------------------------------------------------------------
// Name: SpinBoxDisplayer::StartDisplay
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void SpinBoxDisplayer::StartDisplay()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SpinBoxDisplayer::DisplayFormatted
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void SpinBoxDisplayer::DisplayFormatted( const QString& )
{
    throw std::runtime_error( __FUNCTION__ ); // $$$$ AGE 2006-02-23: 
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
    edit_->setValue( value );
}

// -----------------------------------------------------------------------------
// Name: SpinBoxDisplayer::Call
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void SpinBoxDisplayer::Call( const float& value )
{
    edit_->setValue( value );
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
    edit_->setSuffix( value );
}

// -----------------------------------------------------------------------------
// Name: SpinBoxDisplayer::Hide
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void SpinBoxDisplayer::Hide()
{
    // NOTHING
}

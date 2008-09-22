// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "CheckBox.h"
#include "moc_CheckBox.cpp"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: CheckBox constructor
// Created: RPD 2008-08-21
// -----------------------------------------------------------------------------
CheckBox::CheckBox( const QString & text, QWidget* parent, const char* name /* = 0 */ )
    : QCheckBox( text, parent, name )
    , previousValue_( 0 )
    , currentValue_( 0 )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: CheckBox destructor
// Created: RPD 2008-08-21
// -----------------------------------------------------------------------------
CheckBox::~CheckBox()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: CheckBox::setValue
// Created: RPD 2008-08-21
// -----------------------------------------------------------------------------
void CheckBox::setValue( int val )
{
    if ( val == 2 )
        QCheckBox::setNoChange();
    else
        QCheckBox::setChecked( val == 1 );
    currentValue_ = val;
}

// -----------------------------------------------------------------------------
// Name: CheckBox::setChecked
// Created: RPD 2008-08-21
// -----------------------------------------------------------------------------
void CheckBox::setChecked( bool state )
{
    QCheckBox::setChecked( state );
    currentValue_ = state;
}

// -----------------------------------------------------------------------------
// Name: CheckBox::Revert
// Created: RPD 2008-08-21
// -----------------------------------------------------------------------------
void CheckBox::Revert()
{
    setValue( previousValue_ );
}

// -----------------------------------------------------------------------------
// Name: CheckBox::Commit
// Created: RPD 2008-08-21
// -----------------------------------------------------------------------------
void CheckBox::Commit()
{
    previousValue_ = currentValue_;
}

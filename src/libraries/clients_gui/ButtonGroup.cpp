// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "ButtonGroup.h"
#include "moc_ButtonGroup.cpp"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: ButtonGroup constructor
// Created: RPD 2008-08-21
// -----------------------------------------------------------------------------
ButtonGroup::ButtonGroup ( int columns, Qt::Orientation o, const QString &title, QWidget* parent /* = 0*/, const char* name /* = 0*/ )
    : Q3ButtonGroup( columns, o, title, parent, name )
    , previousId_( 0 )
    , currentId_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ButtonGroup destructor
// Created: RPD 2008-08-21
// -----------------------------------------------------------------------------
ButtonGroup::~ButtonGroup()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ButtonGroup::setButton
// Created: RPD 2008-08-21
// -----------------------------------------------------------------------------
void ButtonGroup::setButton( int id )
{
    Q3ButtonGroup::setButton( id );
    currentId_ = id;
}

// -----------------------------------------------------------------------------
// Name: ButtonGroup::Revert
// Created: RPD 2008-08-21
// -----------------------------------------------------------------------------
void ButtonGroup::Revert()
{
    setButton( previousId_ );
}

// -----------------------------------------------------------------------------
// Name: ButtonGroup::Commit
// Created: RPD 2008-08-21
// -----------------------------------------------------------------------------
void ButtonGroup::Commit()
{
    previousId_ = currentId_;
}

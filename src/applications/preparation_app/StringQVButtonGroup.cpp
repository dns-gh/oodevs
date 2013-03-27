// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "StringQVButtonGroup.h"
#include "moc_StringQVButtonGroup.cpp"
#include "clients_gui/RichCheckBox.h"

// -----------------------------------------------------------------------------
// Name: StringQVButtonGroup constructor
// Created: HBD 2010-09-16
// -----------------------------------------------------------------------------
StringQVButtonGroup::StringQVButtonGroup( const QString& title, QWidget* parent )
    : Q3VButtonGroup( title, parent )
{
    connect( this, SIGNAL( clicked( int ) ), this, SLOT ( OnClicked( int )) );
}

// -----------------------------------------------------------------------------
// Name: StringQVButtonGroup destructor
// Created: HBD 2010-09-16
// -----------------------------------------------------------------------------
StringQVButtonGroup::~StringQVButtonGroup()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: StringQVButtonGroup::insert
// Created: HBD 2010-09-16
// -----------------------------------------------------------------------------
void StringQVButtonGroup::InsertCheckbox( const std::string& objectName, const std::string& text )
{
    new gui::RichCheckBox( objectName.c_str(), text.c_str(), this );
    values_.push_back( text );
}

// -----------------------------------------------------------------------------
// Name: StringQVButtonGroup::OnClicked
// Created: HBD 2010-09-16
// -----------------------------------------------------------------------------
void StringQVButtonGroup::OnClicked( int id )
{
    emit clicked( values_[ id ] );
}

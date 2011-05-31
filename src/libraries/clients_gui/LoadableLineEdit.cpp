// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "LoadableLineEdit.h"
#include "ObjectPrototypeLoader_ABC.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: LoadableLineEdit constructor
// Created: BCI 2011-05-09
// -----------------------------------------------------------------------------
LoadableLineEdit::LoadableLineEdit( QWidget* parent, const QString& probableName )
: LoadableFieldTemplate< QLineEdit >( parent, probableName )
{
    SetDefaultValueWidget( new QLineEdit( this ) );
}

// -----------------------------------------------------------------------------
// Name: LoadableLineEdit destructor
// Created: BCI 2011-05-09
// -----------------------------------------------------------------------------
LoadableLineEdit::~LoadableLineEdit()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LoadableLineEdit::text
// Created: BCI 2011-05-09
// -----------------------------------------------------------------------------
QString LoadableLineEdit::text() const
{
    if( currentLoader_ )
    {
        QString field = GetField();
        if( !field.isNull() )
            return currentLoader_->GetCurrentFieldValueAsString( field );
    }
    return GetDefaultValueWidget()->text();
}

// -----------------------------------------------------------------------------
// Name: LoadableLineEdit::clear
// Created: BCI 2011-05-10
// -----------------------------------------------------------------------------
void LoadableLineEdit::clear()
{
    GetDefaultValueWidget()->setText( QString::null );
}

// -----------------------------------------------------------------------------
// Name: LoadableLineEdit::setText
// Created: BCI 2011-05-12
// -----------------------------------------------------------------------------
void LoadableLineEdit::setText( const QString& s )
{
    GetDefaultValueWidget()->setText( s );
}

// -----------------------------------------------------------------------------
// Name: LoadableLineEdit::setValidator
// Created: BCI 2011-05-12
// -----------------------------------------------------------------------------
void LoadableLineEdit::setValidator( const QValidator* v )
{
    GetDefaultValueWidget()->setValidator( v );
}

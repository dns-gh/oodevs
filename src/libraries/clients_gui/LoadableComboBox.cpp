// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "LoadableComboBox.h"
#include "ObjectPrototypeLoader_ABC.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: LoadableComboBox constructor
// Created: JSR 2011-07-07
// -----------------------------------------------------------------------------
LoadableComboBox::LoadableComboBox( bool rw, QWidget* parent, const QString& probableName /*= QString()*/ )
    : LoadableFieldTemplate< QComboBox >( parent, probableName )
{
    SetDefaultValueWidget( new QComboBox( rw, this ) );
}

// -----------------------------------------------------------------------------
// Name: LoadableComboBox destructor
// Created: JSR 2011-07-07
// -----------------------------------------------------------------------------
LoadableComboBox::~LoadableComboBox()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LoadableComboBox::currentText
// Created: JSR 2011-07-07
// -----------------------------------------------------------------------------
QString LoadableComboBox::currentText() const
{
    if( currentLoader_ )
    {
        QString field = GetField();
        if( !field.isNull() )
            return currentLoader_->GetCurrentFieldValueAsString( field );
    }
    return GetDefaultValueWidget()->currentText();
}

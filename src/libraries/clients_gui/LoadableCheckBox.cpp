// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "LoadableCheckBox.h"
#include "ObjectPrototypeLoader_ABC.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: LoadableCheckBox constructor
// Created: BCI 2011-05-09
// -----------------------------------------------------------------------------
LoadableCheckBox::LoadableCheckBox( const QString& text, QWidget* parent, const QString& probableName )
    : LoadableFieldTemplate< QCheckBox >( parent, probableName )
{
    SetDefaultValueWidget( new QCheckBox( text, this ) );
}

// -----------------------------------------------------------------------------
// Name: LoadableCheckBox destructor
// Created: BCI 2011-05-09
// -----------------------------------------------------------------------------
LoadableCheckBox::~LoadableCheckBox()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LoadableCheckBox::isChecked
// Created: BCI 2011-05-12
// -----------------------------------------------------------------------------
bool LoadableCheckBox::isChecked() const
{
    if( currentLoader_ )
    {
        QString field = GetField();
        if( !field.isNull() )
            return currentLoader_->GetCurrentFieldValueAsBool( field );
    }
    return GetDefaultValueWidget()->isOn();
}

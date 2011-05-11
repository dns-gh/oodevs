// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "LoadableSpinBox.h"
#include "ObjectPrototypeLoader_ABC.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: LoadableSpinBox constructor
// Created: BCI 2011-05-09
// -----------------------------------------------------------------------------
LoadableSpinBox::LoadableSpinBox( int minValue, int maxValue, int step, QWidget* parent, const QString& probableName )
: LoadableFieldTemplate< QSpinBox >( parent, probableName )
{
    SetDefaultValueWidget( new QSpinBox( minValue, maxValue, step, this ) );
}

// -----------------------------------------------------------------------------
// Name: LoadableSpinBox destructor
// Created: BCI 2011-05-09
// -----------------------------------------------------------------------------
LoadableSpinBox::~LoadableSpinBox()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LoadableSpinBox::text
// Created: BCI 2011-05-09
// -----------------------------------------------------------------------------
int LoadableSpinBox::value() const
{
    if( currentLoader_ )
    {
        QString field = GetField();
        if( !field.isNull() ) 
            return currentLoader_->GetCurrentFieldValueAsInt( field );
    }
    return GetDefaultValueWidget()->value();
}

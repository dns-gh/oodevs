// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "LoadableTimeEdit.h"
#include "DurationEditor.h"
#include "ObjectPrototypeLoader_ABC.h"
#include "ObjectNameManager.h"
#include "RichWidget.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: LoadableTimeEdit constructor
// Created: BCI 2011-05-09
// -----------------------------------------------------------------------------
LoadableTimeEdit::LoadableTimeEdit( const QString& objectName, QWidget* parent )
    : LoadableFieldTemplate< QSpinBox >( parent, objectName )
{
    SetDefaultValueWidget( new RichWidget< DurationEditor >( "default" + objectName ,this ) );
}

// -----------------------------------------------------------------------------
// Name: LoadableTimeEdit destructor
// Created: BCI 2011-05-09
// -----------------------------------------------------------------------------
LoadableTimeEdit::~LoadableTimeEdit()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LoadableTimeEdit::Seconds
// Created: JSR 2014-02-24
// -----------------------------------------------------------------------------
int LoadableTimeEdit::Seconds() const
{
    if( currentLoader_ )
    {
        QString field = GetField();
        if( !field.isNull() )
            return currentLoader_->GetCurrentFieldValueAsInt( field );
    }
    return GetDefaultValueWidget()->value();
}


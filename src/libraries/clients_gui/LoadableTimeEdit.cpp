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
#include "ObjectPrototypeLoader_ABC.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: LoadableTimeEdit constructor
// Created: BCI 2011-05-09
// -----------------------------------------------------------------------------
LoadableTimeEdit::LoadableTimeEdit( QWidget* parent, const QString& probableName )
    : LoadableFieldTemplate< QTimeEdit >( parent, probableName )
{
    SetDefaultValueWidget( new QTimeEdit( this ) );
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
// Name: LoadableTimeEdit::time
// Created: BCI 2011-05-12
// -----------------------------------------------------------------------------
QTime LoadableTimeEdit::time() const
{
    if( currentLoader_ )
    {
        QString field = GetField();
        if( !field.isNull() )
            return QTime().addSecs( currentLoader_->GetCurrentFieldValueAsInt( field ) );
    }
    return GetDefaultValueWidget()->time();
}


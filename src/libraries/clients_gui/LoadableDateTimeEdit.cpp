// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "LoadableDateTimeEdit.h"
#include "ObjectPrototypeLoader_ABC.h"
#include "ObjectNameManager.h"
#include "RichWidget.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: LoadableDateTimeEdit constructor
// Created: JSR 2013-09-12
// -----------------------------------------------------------------------------
LoadableDateTimeEdit::LoadableDateTimeEdit( const QString& objectName, QWidget* parent )
    : LoadableFieldTemplate< QDateTimeEdit >( parent, objectName )
{
    SetDefaultValueWidget( new RichWidget< QDateTimeEdit >( "default" + objectName ,this ) );
}

// -----------------------------------------------------------------------------
// Name: LoadableDateTimeEdit destructor
// Created: JSR 2013-09-12
// -----------------------------------------------------------------------------
LoadableDateTimeEdit::~LoadableDateTimeEdit()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LoadableDateTimeEdit::dateTime
// Created: JSR 2013-09-12
// -----------------------------------------------------------------------------
QDateTime LoadableDateTimeEdit::dateTime() const
{
    if( currentLoader_ )
    {
        QString field = GetField();
        if( !field.isNull() )
            return QDateTime().addSecs( currentLoader_->GetCurrentFieldValueAsInt( field ) );
    }
    return GetDefaultValueWidget()->dateTime();
}

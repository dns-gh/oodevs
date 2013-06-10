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
#include "RichWidget.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: LoadableComboBox constructor
// Created: JSR 2011-07-07
// -----------------------------------------------------------------------------
LoadableComboBox::LoadableComboBox(  const QString& objectName, bool rw, QWidget* parent )
    : LoadableFieldTemplate< RichWidget< QComboBox > >( parent, objectName )
{
    RichWidget< QComboBox >* comboBox = new RichWidget< QComboBox >( objectName + "ComboBox", this );
    comboBox->setEditable( rw );
    if( rw )
        comboBox->lineEdit()->setObjectName( objectName + "LineEdit" );
    SetDefaultValueWidget( comboBox );
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

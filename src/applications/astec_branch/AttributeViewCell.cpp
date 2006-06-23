// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "AttributeViewCell.h"
#include "DataDictionary.h"
#include "AttributeViewCellEditor.h"

// -----------------------------------------------------------------------------
// Name: AttributeViewCell constructor
// Created: AGE 2006-06-22
// -----------------------------------------------------------------------------
AttributeViewCell::AttributeViewCell( QTable* parent, const DataDictionary& dictionary )
    : QTableItem( parent, WhenCurrent )
    , dictionary_( dictionary )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AttributeViewCell destructor
// Created: AGE 2006-06-22
// -----------------------------------------------------------------------------
AttributeViewCell::~AttributeViewCell()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AttributeViewCell::createEditor
// Created: AGE 2006-06-22
// -----------------------------------------------------------------------------
QWidget* AttributeViewCell::createEditor() const
{
    AttributeViewCellEditor* edit = new AttributeViewCellEditor( table()->viewport(), dictionary_, const_cast< AttributeViewCell* >( this ) );
    edit->setText( text() );
    return edit;
}

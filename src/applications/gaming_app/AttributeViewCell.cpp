// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "AttributeViewCell.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "AttributeViewCellEditor.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AttributeViewCell constructor
// Created: AGE 2006-06-22
// -----------------------------------------------------------------------------
AttributeViewCell::AttributeViewCell( QTable* parent, PropertiesDictionary& dictionary )
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
    return new AttributeViewCellEditor( table()->viewport(), dictionary_, const_cast< AttributeViewCell* >( this ) );
}

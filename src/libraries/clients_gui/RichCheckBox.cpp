// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "RichCheckBox.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: RichCheckBox constructor
// Created: NPT 2013-03-12
// -----------------------------------------------------------------------------
RichCheckBox::RichCheckBox( const QString& objectName, QWidget* parent /*= 0*/ )
    : RichWidget< QCheckBox >( objectName, parent )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RichCheckBox constructor
// Created: NPT 2013-03-12
// -----------------------------------------------------------------------------
RichCheckBox::RichCheckBox( const QString& objectName, const QString& text, QWidget* parent /* = 0 */ )
    : RichWidget< QCheckBox >( objectName, parent )
{
    setText( text );
}

// -----------------------------------------------------------------------------
// Name: RichCheckBox destructor
// Created: NPT 2013-03-12
// -----------------------------------------------------------------------------
RichCheckBox::~RichCheckBox()
{
    // NOTHING
}

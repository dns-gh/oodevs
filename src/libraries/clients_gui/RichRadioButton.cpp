// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "RichRadioButton.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: RichRadioButton constructor
// Created: ABR 2013-06-10
// -----------------------------------------------------------------------------
RichRadioButton::RichRadioButton( const QString& objectName, QWidget* parent /*= 0*/ )
    : RichWidget< QRadioButton >( objectName, parent )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RichRadioButton constructor
// Created: NPT 2013-03-14
// -----------------------------------------------------------------------------
RichRadioButton::RichRadioButton( const QString& objectName, const QString& text, QWidget* parent /*= 0*/ )
    : RichWidget< QRadioButton >( objectName, parent )
{
    setText( text );
}

// -----------------------------------------------------------------------------
// Name: RichRadioButton destructor
// Created: NPT 2013-03-14
// -----------------------------------------------------------------------------
RichRadioButton::~RichRadioButton()
{
    // NOTHING
}

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "RichPushButton.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: RichPushButton constructor
// Created: NPT 2013-03-12
// -----------------------------------------------------------------------------
RichPushButton::RichPushButton( const QString& objectName, const QString& text, QWidget* parent )
    : RichWidget< QPushButton >( objectName, parent )
{
    setText( text );
}

// -----------------------------------------------------------------------------
// Name: RichPushButton constructor
// Created: NPT 2013-03-12
// -----------------------------------------------------------------------------
RichPushButton::RichPushButton( const QString& objectName, const QIcon & icon, const QString& text, QWidget* parent /*= 0*/ )
    : RichWidget< QPushButton >( objectName, parent )
{
    setText( text );
    setIcon( icon );
}

// -----------------------------------------------------------------------------
// Name: RichPushButton destructor
// Created: NPT 2013-03-12
// -----------------------------------------------------------------------------
RichPushButton::~RichPushButton()
{
    // NOTHING
}

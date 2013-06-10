// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "RichGroupBox.h"
#include "ObjectNameManager.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: RichGroupBox constructor
// Created: ABR 2011-11-21
// -----------------------------------------------------------------------------
RichGroupBox::RichGroupBox( const QString& objectName, QWidget* parent /* = 0 */ )
    : RichWarnWidget< QGroupBox >( objectName, parent )
{
    setSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding );
}

// -----------------------------------------------------------------------------
// Name: RichGroupBox constructor
// Created: ABR 2011-11-21
// -----------------------------------------------------------------------------
RichGroupBox::RichGroupBox( const QString& objectName, const QString& title, QWidget* parent /* = 0 */ )
    : RichWarnWidget< QGroupBox >( objectName, parent )
{
    setTitle( title );
    setSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding );
}

// -----------------------------------------------------------------------------
// Name: RichGroupBox destructor
// Created: ABR 2011-11-21
// -----------------------------------------------------------------------------
RichGroupBox::~RichGroupBox()
{
    // NOTHING
}

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "PreferencePanel_ABC.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: PreferencePanel_ABC constructor
// Created: SBO 2007-01-03
// -----------------------------------------------------------------------------
PreferencePanel_ABC::PreferencePanel_ABC( QWidget* parent, const char* name )
    : QScrollArea( parent )
{
    setName( name );
    setFrameStyle( QFrame::Box );
    setWidgetResizable( true );
}

// -----------------------------------------------------------------------------
// Name: PreferencePanel_ABC destructor
// Created: SBO 2007-01-03
// -----------------------------------------------------------------------------
PreferencePanel_ABC::~PreferencePanel_ABC()
{
    // NOTHING
}

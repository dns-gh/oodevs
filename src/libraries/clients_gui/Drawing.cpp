// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "Drawing.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: Drawing constructor
// Created: SBO 2008-05-29
// -----------------------------------------------------------------------------
Drawing::Drawing( kernel::Controller& controller, unsigned long id, const QString& name, E_Dash_style style )
    : gui::EntityImplementation< Drawing_ABC >( controller, id, name )
    , dashStyle_( style )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Drawing destructor
// Created: SBO 2008-05-29
// -----------------------------------------------------------------------------
Drawing::~Drawing()
{
    // NOTHING
}

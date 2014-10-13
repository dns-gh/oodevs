// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "ShapeHandler_ABC.h"

#include "SimpleLocationDrawer.h"
#include "clients_kernel/Location_ABC.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: ShapeHandler_ABC constructor
// Created: SBO 2008-06-03
// -----------------------------------------------------------------------------
ShapeHandler_ABC::ShapeHandler_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ShapeHandler_ABC destructor
// Created: SBO 2008-06-03
// -----------------------------------------------------------------------------
ShapeHandler_ABC::~ShapeHandler_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ShapeHandler_ABC::Draw
// Created: SBO 2008-06-03
// -----------------------------------------------------------------------------
void ShapeHandler_ABC::Draw( const kernel::Location_ABC& location, const geometry::Rectangle2f&, const GLView_ABC& tools ) const
{
    gui::SimpleLocationDrawer drawer( tools );
    location.Accept( drawer );
}

// -----------------------------------------------------------------------------
// Name: ShapeHandler_ABC::Reset
// Created: ABR 2013-02-04
// -----------------------------------------------------------------------------
void ShapeHandler_ABC::Reset()
{
    // NOTHING
}

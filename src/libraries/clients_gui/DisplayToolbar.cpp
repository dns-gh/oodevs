// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "DisplayToolbar.h"
#include "BooleanOptionButton.h"
#include "resources.h"
#include "Tools.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Options.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: DisplayToolbar constructor
// Created: SBO 2007-03-05
// -----------------------------------------------------------------------------
DisplayToolbar::DisplayToolbar( QMainWindow* pParent, kernel::Controllers& controllers )
    : QToolBar( pParent, "display toolbar" )
{
    setLabel( tools::translate( "DisplayToolbar", "Display toolbar" ) );
    new BooleanOptionButton( MAKE_ICON( threed ), tools::translate( "DisplayToolbar", "3D" ), this, controllers.options_, "3D" );
}

// -----------------------------------------------------------------------------
// Name: DisplayToolbar destructor
// Created: SBO 2007-03-05
// -----------------------------------------------------------------------------
DisplayToolbar::~DisplayToolbar()
{
    // NOTHING
}

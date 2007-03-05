// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "DisplayToolbar.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Options.h"
#include "clients_gui/BooleanOptionButton.h"
#include "clients_gui/resources.h"

// -----------------------------------------------------------------------------
// Name: DisplayToolbar constructor
// Created: SBO 2007-03-05
// -----------------------------------------------------------------------------
DisplayToolbar::DisplayToolbar( QMainWindow* pParent, kernel::Controllers& controllers )
    : QToolBar( pParent, "display toolbar" )
{
    setLabel( tr( "Display toolbar" ) );
    new gui::BooleanOptionButton( MAKE_ICON( threed ), tr( "3D" ), this, controllers.options_, "3D" );
}

// -----------------------------------------------------------------------------
// Name: DisplayToolbar destructor
// Created: SBO 2007-03-05
// -----------------------------------------------------------------------------
DisplayToolbar::~DisplayToolbar()
{
    // NOTHING
}

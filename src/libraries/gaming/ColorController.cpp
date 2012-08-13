// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ColorController.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Color_ABC.h"

// -----------------------------------------------------------------------------
// Name: ColorController constructor
// Created: LGY 2011-06-27
// -----------------------------------------------------------------------------
ColorController::ColorController( kernel::Controllers& controllers )
    : controllers_( controllers )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ColorController destructor
// Created: LGY 2011-06-27
// -----------------------------------------------------------------------------
ColorController::~ColorController()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ColorController::UpdateHierarchies
// Created: LDC 2012-05-04
// -----------------------------------------------------------------------------
void ColorController::UpdateHierarchies( const kernel::Entity_ABC& /*entity*/ )
{
    // NOTHING
}

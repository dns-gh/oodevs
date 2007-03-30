// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "TacticalList.h"
#include "TacticalListView.h"
#include "clients_gui/EntitySearchBox.h"
#include "clients_kernel/Entity_ABC.h"

// -----------------------------------------------------------------------------
// Name: TacticalList constructor
// Created: AGE 2006-10-20
// -----------------------------------------------------------------------------
TacticalList::TacticalList( kernel::Controllers& controllers, Publisher_ABC& publisher, gui::ItemFactory_ABC& factory, const kernel::Profile_ABC& profile, gui::EntitySymbols& icons )
{
    new gui::EntitySearchBox< kernel::Entity_ABC >( this, controllers );
    new TacticalListView( this, controllers, publisher, factory, profile, icons );
}

// -----------------------------------------------------------------------------
// Name: TacticalList destructor
// Created: AGE 2006-10-20
// -----------------------------------------------------------------------------
TacticalList::~TacticalList()
{
    // NOTHING
}

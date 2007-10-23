// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "IntelligenceList.h"
#include "EntitySearchBox.h"
#include "IntelligenceListView.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: IntelligenceList constructor
// Created: SBO 2007-10-19
// -----------------------------------------------------------------------------
IntelligenceList::IntelligenceList( kernel::Controllers& controllers, ItemFactory_ABC& factory, EntitySymbols& icons, const kernel::Profile_ABC& profile )
{
    new EntitySearchBox< kernel::Intelligence_ABC >( this, controllers );
    new IntelligenceListView( this, controllers, factory, icons, profile );
}

// -----------------------------------------------------------------------------
// Name: IntelligenceList destructor
// Created: SBO 2007-10-19
// -----------------------------------------------------------------------------
IntelligenceList::~IntelligenceList()
{
    // NOTHING
}

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "PopulationList.h"
#include "PopulationListView.h"
#include "EntitySearchBox.h"
#include "clients_kernel/Population_ABC.h"

// -----------------------------------------------------------------------------
// Name: PopulationList constructor
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
PopulationList::PopulationList( Controllers& controllers, ItemFactory_ABC& factory )
{
    new EntitySearchBox< Population_ABC >( this, controllers );
    new PopulationListView( this, controllers, factory );
}

// -----------------------------------------------------------------------------
// Name: PopulationList destructor
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
PopulationList::~PopulationList()
{
    // NOTHING
}

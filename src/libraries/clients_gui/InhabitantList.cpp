// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "InhabitantList.h"
#include "InhabitantListView.h"
#include "EntitySearchBox.h"
#include "clients_kernel/Population_ABC.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: InhabitantList constructor
// Created: SLG 2010-11-30
// -----------------------------------------------------------------------------
InhabitantList::InhabitantList( Controllers& controllers, ItemFactory_ABC& factory, const kernel::Profile_ABC& profile )
{
    new EntitySearchBox< Inhabitant_ABC >( this, controllers );
    new InhabitantListView( this, controllers, factory, profile );
}

// -----------------------------------------------------------------------------
// Name: InhabitantList destructor
// Created: SLG 2010-11-30
// -----------------------------------------------------------------------------
InhabitantList::~InhabitantList()
{
    // NOTHING
}

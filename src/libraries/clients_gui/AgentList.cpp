// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "AgentList.h"
#include "AgentListView.h"
#include "EntitySearchBox.h"
#include "clients_kernel/Agent_ABC.h"

// -----------------------------------------------------------------------------
// Name: AgentList constructor
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
AgentList::AgentList( Controllers& controllers, Publisher_ABC& publisher, ItemFactory_ABC& factory )
{
    new EntitySearchBox< Agent_ABC >( this, controllers );
    new AgentListView( this, controllers, publisher, factory );
}

// -----------------------------------------------------------------------------
// Name: AgentList destructor
// Created: AGE 2006-08-10
// -----------------------------------------------------------------------------
AgentList::~AgentList()
{
    // NOTHING
}

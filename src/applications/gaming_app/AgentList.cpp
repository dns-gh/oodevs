// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "AgentList.h"
#include "AgentListView.h"
#include "clients_gui/EntitySearchBox.h"
#include "clients_kernel/Agent_ABC.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: AgentList constructor
// Created: AGE 2006-08-29
// -----------------------------------------------------------------------------
AgentList::AgentList( Controllers& controllers, ActionPublisher& actionPublisher, actions::ActionsModel& actionsModel, const StaticModel& staticModel, const Simulation& simulation, ItemFactory_ABC& factory, const kernel::Profile_ABC& profile, gui::EntitySymbols& icons )
{
    new EntitySearchBox< Agent_ABC >( this, controllers );
    new AgentListView( this, controllers, actionPublisher, actionsModel, staticModel, simulation, factory, profile, icons );
}

// -----------------------------------------------------------------------------
// Name: AgentList destructor
// Created: AGE 2006-08-29
// -----------------------------------------------------------------------------
AgentList::~AgentList()
{
    // NOTHING
}

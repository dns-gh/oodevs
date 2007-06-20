// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "CreationPanels.h"
#include "PopulationsPanel.h"
#include "preparation/StaticModel.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_gui/UnitsPanel.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: CreationPanels constructor
// Created: SBO 2006-08-28
// -----------------------------------------------------------------------------
CreationPanels::CreationPanels( QWidget* parent, Controllers& controllers, const StaticModel& staticModel, ItemFactory_ABC& factory )
    : Panels( parent )
{
    AddPanel( new gui::UnitsPanel ( this, *this, controllers, staticModel.types_, factory ) );
    AddPanel( new PopulationsPanel( this, *this, controllers, (Resolver< PopulationType >&)( staticModel.types_ ), factory ) );
}

// -----------------------------------------------------------------------------
// Name: CreationPanels destructor
// Created: SBO 2006-08-28
// -----------------------------------------------------------------------------
CreationPanels::~CreationPanels()
{
    // NOTHING
}

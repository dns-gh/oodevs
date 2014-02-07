// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "InfoDock.h"
#include "InfoWidget.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Tools.h"

// -----------------------------------------------------------------------------
// Name: InfoDock constructor
// Created: SBO 2007-02-02
// -----------------------------------------------------------------------------
InfoDock::InfoDock( QWidget* parent,
                    kernel::Controllers& controllers,
                    const tools::ExerciseConfig& config,
                    const kernel::Profile_ABC& profile,
                    const gui::EntitySymbols& icons,
                    gui::ItemFactory_ABC& itemFactory,
                    gui::DisplayExtractor& extractor,
                    Model& model,
                    const Simulation& simulation,
                    SimulationController& simulationController )
    : gui::RichDockWidget( controllers, parent, "info" )
{
    setObjectName( "info" );
    setWidget( new InfoWidget( this, controllers_, config, profile, icons, itemFactory, extractor, model, simulation, simulationController ) );
    setCaption( tools::translate( "InfoDock", "Info" ) );
    hide();
}

// -----------------------------------------------------------------------------
// Name: InfoDock destructor
// Created: SBO 2007-02-02
// -----------------------------------------------------------------------------
InfoDock::~InfoDock()
{
    // NOTHING
}

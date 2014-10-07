// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "MagicOrdersInterface.h"
#include "UnitMagicOrdersInterface.h"
#include "PopulationMagicOrdersInterface.h"
#include "ObjectMagicOrdersInterface.h"
// LTO begin
#include "KnowledgeGroupMagicOrdersInterface.h"
#include "clients_kernel/AgentTypes.h"
#include "gaming/StaticModel.h"
// LTO end

// -----------------------------------------------------------------------------
// Name: MagicOrdersInterface constructor
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
MagicOrdersInterface::MagicOrdersInterface( QWidget* parent,
                                            kernel::Controllers& controllers,
                                            actions::ActionsModel& actionsModel,
                                            const StaticModel& staticModel,
                                            const kernel::Time_ABC& simulation,
                                            const std::shared_ptr< gui::ParametersLayer >& layer,
                                            const kernel::Profile_ABC& profile,
                                            gui::GlSelector& selector )
{
    new UnitMagicOrdersInterface( parent, controllers, actionsModel, staticModel, simulation, layer, profile, selector );
    new PopulationMagicOrdersInterface( parent, controllers, actionsModel, staticModel, simulation, layer, profile, selector );
    new ObjectMagicOrdersInterface( parent, controllers, actionsModel, profile );
    new KnowledgeGroupMagicOrdersInterface( parent, controllers, actionsModel, staticModel, simulation, profile, staticModel.types_ ); // LTO
}

// -----------------------------------------------------------------------------
// Name: MagicOrdersInterface destructor
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
MagicOrdersInterface::~MagicOrdersInterface()
{
    // NOTHING
}

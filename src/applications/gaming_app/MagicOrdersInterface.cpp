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
#include "KnowledgeGroupMagicOrdersInterface.h"
#include "clients_kernel/AgentTypes.h"
#include "gaming/StaticModel.h"

// -----------------------------------------------------------------------------
// Name: MagicOrdersInterface constructor
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
MagicOrdersInterface::MagicOrdersInterface( QWidget* parent, kernel::Controllers& controllers, Publisher_ABC& publisher, const StaticModel& staticModel, gui::ParametersLayer& layer, const kernel::Profile_ABC& profile )
{
    new UnitMagicOrdersInterface( parent, controllers, publisher, staticModel, layer, profile );
    new PopulationMagicOrdersInterface( parent, controllers, publisher, staticModel, layer, profile );
    new ObjectMagicOrdersInterface( parent, controllers, publisher, profile );
    new KnowledgeGroupMagicOrdersInterface( parent, controllers, publisher, profile, staticModel.types_ );
}

// -----------------------------------------------------------------------------
// Name: MagicOrdersInterface destructor
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
MagicOrdersInterface::~MagicOrdersInterface()
{
    // NOTHING
}

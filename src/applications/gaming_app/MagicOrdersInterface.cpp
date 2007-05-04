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

// -----------------------------------------------------------------------------
// Name: MagicOrdersInterface constructor
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
MagicOrdersInterface::MagicOrdersInterface( QWidget* parent, kernel::Controllers& controllers, Publisher_ABC& publisher, const StaticModel& staticModel, gui::ParametersLayer& layer, const kernel::Profile_ABC& profile )
{
    new UnitMagicOrdersInterface( parent, controllers, publisher, staticModel, layer, profile );
    new PopulationMagicOrdersInterface( parent, controllers, publisher, staticModel, layer, profile );
    new ObjectMagicOrdersInterface( parent, controllers, publisher, profile );
}

// -----------------------------------------------------------------------------
// Name: MagicOrdersInterface destructor
// Created: SBO 2007-05-04
// -----------------------------------------------------------------------------
MagicOrdersInterface::~MagicOrdersInterface()
{
    // NOTHING
}

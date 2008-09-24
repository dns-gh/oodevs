// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "tic_plugin_pch.h"
#include "ExtensionFactory.h"
#include "TicExtension.h"
#include "dispatcher/Agent.h"

using namespace plugins::tic;

// -----------------------------------------------------------------------------
// Name: ExtensionFactory constructor
// Created: AGE 2008-03-31
// -----------------------------------------------------------------------------
ExtensionFactory::ExtensionFactory( const kernel::CoordinateConverter_ABC& converter, float timeStep )
    : converter_( converter )
    , timeStep_ ( timeStep )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ExtensionFactory destructor
// Created: AGE 2008-03-31
// -----------------------------------------------------------------------------
ExtensionFactory::~ExtensionFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ExtensionFactory::Create
// Created: AGE 2008-03-31
// -----------------------------------------------------------------------------
void ExtensionFactory::Create( dispatcher::Agent& entity )
{
    entity.Attach< TicExtension_ABC >( *new TicExtension( entity, converter_, timeStep_ ) );
}

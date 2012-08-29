// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "ReplayExtensionFactory.h"
#include "dispatcher/ReplaySynchronisations.h"
#include "clients_kernel/Entity_ABC.h"

using namespace plugins::replay;

// -----------------------------------------------------------------------------
// Name: ReplayExtensionFactory constructor
// Created: SBO 2008-02-14
// -----------------------------------------------------------------------------
ReplayExtensionFactory::ReplayExtensionFactory( const dispatcher::ReplayModel_ABC& model )
    : model_( model )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ReplayExtensionFactory destructor
// Created: SBO 2008-02-14
// -----------------------------------------------------------------------------
ReplayExtensionFactory::~ReplayExtensionFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ReplayExtensionFactory::Create
// Created: SBO 2008-02-14
// -----------------------------------------------------------------------------
void ReplayExtensionFactory::Create( kernel::EntityBase_ABC& entity )
{
    entity.Attach( *new dispatcher::ReplaySynchronisations( model_, entity ) );
}

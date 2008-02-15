// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "ReplayExtensionFactory.h"
#include "ReplaySynchronisations.h"
#include "Entity_ABC.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: ReplayExtensionFactory constructor
// Created: SBO 2008-02-14
// -----------------------------------------------------------------------------
ReplayExtensionFactory::ReplayExtensionFactory( const ReplayModel_ABC& model )
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
void ReplayExtensionFactory::Create( Entity_ABC& entity )
{
    entity.Attach( *new ReplaySynchronisations( model_, entity ) );
}

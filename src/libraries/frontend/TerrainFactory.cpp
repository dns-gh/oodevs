// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_pch.h"
#include "TerrainFactory.h"
#include "Terrain.h"
#include "clients_kernel/Controllers.h"

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: TerrainFactory constructor
// Created: SBO 2007-01-29
// -----------------------------------------------------------------------------
TerrainFactory::TerrainFactory( kernel::Controllers& controllers )
    : controllers_( controllers )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TerrainFactory destructor
// Created: SBO 2007-01-29
// -----------------------------------------------------------------------------
TerrainFactory::~TerrainFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TerrainFactory::Create
// Created: SBO 2007-01-29
// -----------------------------------------------------------------------------
Terrain* TerrainFactory::Create( const ASN1T_MsgTerrainCreation& message )
{
    return new Terrain( message, controllers_.controller_ );
}

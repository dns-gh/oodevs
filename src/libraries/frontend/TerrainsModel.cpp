// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_pch.h"
#include "TerrainsModel.h"
#include "TerrainFactory_ABC.h"
#include "Terrain.h"

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: TerrainsModel constructor
// Created: SBO 2007-01-29
// -----------------------------------------------------------------------------
TerrainsModel::TerrainsModel( TerrainFactory_ABC& factory )
    : factory_( factory )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TerrainsModel destructor
// Created: SBO 2007-01-29
// -----------------------------------------------------------------------------
TerrainsModel::~TerrainsModel()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: TerrainsModel::CreateTerrain
// Created: SBO 2007-02-01
// -----------------------------------------------------------------------------
void TerrainsModel::CreateTerrain( const ASN1T_MsgTerrainCreation& message )
{
    std::auto_ptr< Terrain > terrain( factory_.Create( message ) );
    Register( terrain->GetName(), *terrain.release() );
}

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "master_pch.h"
#include "Terrain.h"

using namespace master;

// -----------------------------------------------------------------------------
// Name: Terrain constructor
// Created: NLD 2007-01-29
// -----------------------------------------------------------------------------
Terrain::Terrain( const DataManager& /*dataManager*/, const Config& /*config*/, const std::string& name )
    : name_( name )
{
    MT_LOG_INFO_MSG( "Terrain loaded : '" << name_ << "'" );
}
    
// -----------------------------------------------------------------------------
// Name: Terrain destructor
// Created: NLD 2007-01-29
// -----------------------------------------------------------------------------
Terrain::~Terrain()
{
    // NOTHING
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: Terrain::SendCreation
// Created: NLD 2007-01-31
// -----------------------------------------------------------------------------
void Terrain::SendCreation( Publisher_ABC& publisher ) const
{
    /*
    */
}

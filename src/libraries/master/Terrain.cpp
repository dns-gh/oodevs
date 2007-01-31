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
#include "Network_Def.h"

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
// ACCESSORS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PhysicalModel::GetName
// Created: NLD 2007-01-31
// -----------------------------------------------------------------------------
const std::string& Terrain::GetName() const
{
    return name_;
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
    AsnMsgOutMasterTerrainCreation asn;
    Send( asn() );
    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Terrain::Send
// Created: NLD 2007-01-31
// -----------------------------------------------------------------------------
void Terrain::Send( ASN1T_Terrain& asn ) const
{
    asn.name = name_.c_str();
}

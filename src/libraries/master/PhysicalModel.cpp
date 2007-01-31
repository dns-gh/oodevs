// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "master_pch.h"
#include "PhysicalModel.h"
#include "Dataset.h"
#include "Network_Def.h"

using namespace master;

// -----------------------------------------------------------------------------
// Name: PhysicalModel constructor
// Created: NLD 2007-01-29
// -----------------------------------------------------------------------------
PhysicalModel::PhysicalModel( const Dataset& dataset, const Config& /*config*/, const std::string& name )
    : dataset_( dataset )
    , name_   ( name )
{
    MT_LOG_INFO_MSG( "Physical model loaded : '" << name_ << "'" );
}
    
// -----------------------------------------------------------------------------
// Name: PhysicalModel destructor
// Created: NLD 2007-01-29
// -----------------------------------------------------------------------------
PhysicalModel::~PhysicalModel()
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
const std::string& PhysicalModel::GetName() const
{
    return name_;
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PhysicalModel::SendCreation
// Created: NLD 2007-01-31
// -----------------------------------------------------------------------------
void PhysicalModel::SendCreation( Publisher_ABC& publisher ) const
{
    AsnMsgOutMasterPhysicalModelCreation asn;
    Send( asn() );
    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: PhysicalModel::Send
// Created: NLD 2007-01-31
// -----------------------------------------------------------------------------
void PhysicalModel::Send( ASN1T_PhysicalModel& asn ) const
{
    asn.name    = name_.c_str();
    asn.dataset = dataset_.GetName().c_str();
}

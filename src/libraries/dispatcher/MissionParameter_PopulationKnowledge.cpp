// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_PopulationKnowledge.h"
#include "ClientPublisher_ABC.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_PopulationKnowledge constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_PopulationKnowledge::MissionParameter_PopulationKnowledge( const ASN1T_MissionParameter& asn )
    : MissionParameter_ABC( asn )
    , populationKnowledge_( asn.value.u.populationKnowledge )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_PopulationKnowledge destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_PopulationKnowledge::~MissionParameter_PopulationKnowledge()
{
    // NOTHING
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MissionParameter_PopulationKnowledge::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_PopulationKnowledge::Send( ASN1T_MissionParameter& asn ) const
{
    asn.null_value                  = bNullValue_;
    asn.value.t                     = T_MissionParameter_value_populationKnowledge;
    asn.value.u.populationKnowledge = populationKnowledge_;
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_PopulationKnowledge::AsnDelete
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_PopulationKnowledge::AsnDelete( ASN1T_MissionParameter& /*asn*/ ) const
{
    // NOTHING
}

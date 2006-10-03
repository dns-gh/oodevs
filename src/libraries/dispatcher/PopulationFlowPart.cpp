// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "PopulationFlowPart.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: PopulationFlowPart constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
PopulationFlowPart::PopulationFlowPart( const Model& /*model*/, const ASN1T_PortionFlux& asn )
    : localisation_( asn.forme )
    , nRelevance_  ( asn.pertinence )
{
}

// -----------------------------------------------------------------------------
// Name: PopulationFlowPart destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
PopulationFlowPart::~PopulationFlowPart()
{

}
       
// -----------------------------------------------------------------------------
// Name: PopulationFlowPart::Send
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void PopulationFlowPart::Send( ASN1T_PortionFlux& asn ) const
{
    asn.pertinence = nRelevance_;
    localisation_.Send( asn.forme );
}

// -----------------------------------------------------------------------------
// Name: PopulationFlowPart::AsnDelete
// Created: NLD 2006-10-03
// -----------------------------------------------------------------------------
void PopulationFlowPart::AsnDelete( ASN1T_PortionFlux& asn )
{
    Localisation::AsnDelete( asn.forme );
}

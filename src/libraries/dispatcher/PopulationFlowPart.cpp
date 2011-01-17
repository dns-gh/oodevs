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
#include "protocol/Protocol.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: PopulationFlowPart constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
PopulationFlowPart::PopulationFlowPart( const sword::FlowPart& asn )
    : localisation_( asn.shape().location() )
    , nRelevance_  ( asn.pertinence() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationFlowPart destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
PopulationFlowPart::~PopulationFlowPart()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationFlowPart::Send
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void PopulationFlowPart::Send( sword::FlowPart& asn ) const
{
    asn.set_pertinence( nRelevance_ );
    localisation_.Send( *asn.mutable_shape()->mutable_location() );
}

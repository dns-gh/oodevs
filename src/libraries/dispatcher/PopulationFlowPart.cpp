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
#include "protocol/protocol.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: PopulationFlowPart constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
PopulationFlowPart::PopulationFlowPart( const MsgsSimToClient::MsgFlowPart& asn )
    : localisation_( asn.forme().location() )
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
void PopulationFlowPart::Send( MsgsSimToClient::MsgFlowPart& asn ) const
{
    asn.set_pertinence( nRelevance_ );
    localisation_.Send( *asn.mutable_forme()->mutable_location() );
}

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_AgentKnowledge.h"
#include "ClientPublisher_ABC.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_AgentKnowledge constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_AgentKnowledge::MissionParameter_AgentKnowledge( const ASN1T_MissionParameter& asn )
    : MissionParameter_ABC( asn )
    , agentKnowledge_     ( asn.value.u.unitKnowledge )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_AgentKnowledge destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_AgentKnowledge::~MissionParameter_AgentKnowledge()
{
    // NOTHING
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MissionParameter_AgentKnowledge::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_AgentKnowledge::Send( ASN1T_MissionParameter& asn ) const
{
    asn.null_value             = bNullValue_;
    asn.value.t                = T_MissionParameter_value_unitKnowledge;
    asn.value.u.unitKnowledge = agentKnowledge_;
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_AgentKnowledge::AsnDelete
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_AgentKnowledge::AsnDelete( ASN1T_MissionParameter& /*asn*/ ) const
{
    // NOTHING
}

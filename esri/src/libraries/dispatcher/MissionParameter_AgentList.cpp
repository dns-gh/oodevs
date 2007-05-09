// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_AgentList.h"
#include "Network_Def.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_AgentList constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_AgentList::MissionParameter_AgentList( const ASN1T_MissionParameter& asn )
    : MissionParameter_ABC( asn )
{
    for( unsigned i = 0; i != asn.value.u.listAgent->n; ++i )
        agents_.push_back( asn.value.u.listAgent->elem[i] );
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_AgentList destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_AgentList::~MissionParameter_AgentList()
{
    // NOTHING
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MissionParameter_AgentList::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_AgentList::Send( ASN1T_MissionParameter& asn ) const
{
    asn.null_value                 = bNullValue_;
    asn.value.t                    = T_MissionParameter_value_listAgent;
    asn.value.u.listAgent = new ASN1T_ListAgent();
    SendContainerValues< ASN1T_ListAgent, ASN1T_Agent, T_OIDVector >( agents_, *asn.value.u.listAgent ); 
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_AgentList::AsnDelete
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_AgentList::AsnDelete( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.listAgent->n > 0 )
        delete [] asn.value.u.listAgent->elem;
    delete asn.value.u.listAgent;
}

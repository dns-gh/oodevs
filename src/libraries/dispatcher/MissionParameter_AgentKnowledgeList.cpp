// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_AgentKnowledgeList.h"
#include "Network_Def.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_AgentKnowledgeList constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_AgentKnowledgeList::MissionParameter_AgentKnowledgeList( const ASN1T_MissionParameter& asn )
    : MissionParameter_ABC( asn )
{
    for( unsigned i = 0; i != asn.value.u.unitKnowledgeList->n; ++i )
        agentKnowledges_.push_back( asn.value.u.unitKnowledgeList->elem[i] );
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_AgentKnowledgeList destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_AgentKnowledgeList::~MissionParameter_AgentKnowledgeList()
{
    // NOTHING
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MissionParameter_AgentKnowledgeList::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_AgentKnowledgeList::Send( ASN1T_MissionParameter& asn ) const
{
    asn.null_value                 = bNullValue_;
    asn.value.t                    = T_MissionParameter_value_unitKnowledgeList;
    asn.value.u.unitKnowledgeList = new ASN1T_UnitKnowledgeList();
    SendContainerValues< ASN1T_UnitKnowledgeList, ASN1T_UnitKnowledge, T_OIDVector >( agentKnowledges_, *asn.value.u.unitKnowledgeList ); 
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_AgentKnowledgeList::AsnDelete
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_AgentKnowledgeList::AsnDelete( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.unitKnowledgeList->n > 0 )
        delete [] asn.value.u.unitKnowledgeList->elem;
    delete asn.value.u.unitKnowledgeList;
}

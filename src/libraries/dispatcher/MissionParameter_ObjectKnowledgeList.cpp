// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_ObjectKnowledgeList.h"
#include "Network_Def.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_ObjectKnowledgeList constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_ObjectKnowledgeList::MissionParameter_ObjectKnowledgeList( const ASN1T_MissionParameter& asn )
    : MissionParameter_ABC( asn )
{
    for( unsigned i = 0; i != asn.value.u.objectKnowledgeList->n; ++i )
        objectKnowledges_.push_back( asn.value.u.objectKnowledgeList->elem[i] );
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_ObjectKnowledgeList destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_ObjectKnowledgeList::~MissionParameter_ObjectKnowledgeList()
{
    // NOTHING
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MissionParameter_ObjectKnowledgeList::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_ObjectKnowledgeList::Send( ASN1T_MissionParameter& asn ) const
{
    asn.null_value                  = bNullValue_;
    asn.value.t                     = T_MissionParameter_value_objectKnowledgeList;
    asn.value.u.objectKnowledgeList = new ASN1T_ObjectKnowledgeList();
    SendContainerValues< ASN1T_ObjectKnowledgeList, ASN1T_ObjectKnowledge, T_OIDVector >( objectKnowledges_, *asn.value.u.objectKnowledgeList ); 
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_ObjectKnowledgeList::AsnDelete
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_ObjectKnowledgeList::AsnDelete( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.objectKnowledgeList->n > 0 )
        delete [] asn.value.u.objectKnowledgeList->elem;
    delete asn.value.u.objectKnowledgeList;
}

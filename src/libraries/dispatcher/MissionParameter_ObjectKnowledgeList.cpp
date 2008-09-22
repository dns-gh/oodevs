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
#include "ClientPublisher_ABC.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_ObjectKnowledgeList constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_ObjectKnowledgeList::MissionParameter_ObjectKnowledgeList( const ASN1T_MissionParameter& asn )
    : MissionParameter_ABC( asn )
{
    std::copy( asn.value.u.objectKnowledgeList->elem, asn.value.u.objectKnowledgeList->elem + asn.value.u.objectKnowledgeList->n
             , std::back_inserter( objectKnowledges_ ) );
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_ObjectKnowledgeList destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_ObjectKnowledgeList::~MissionParameter_ObjectKnowledgeList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_ObjectKnowledgeList::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_ObjectKnowledgeList::Send( ASN1T_MissionParameter& asn ) const
{
    asn.null_value = bNullValue_;
    asn.value.t = T_MissionParameter_value_objectKnowledgeList;
    asn.value.u.objectKnowledgeList = new ASN1T_ObjectKnowledgeList();
    {
        asn.value.u.objectKnowledgeList->n = objectKnowledges_.size();
        asn.value.u.objectKnowledgeList->elem = (int*)( objectKnowledges_.empty() ? 0 : &objectKnowledges_.front() );
    }
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_ObjectKnowledgeList::AsnDelete
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_ObjectKnowledgeList::AsnDelete( ASN1T_MissionParameter& asn ) const
{
    delete asn.value.u.objectKnowledgeList;
}

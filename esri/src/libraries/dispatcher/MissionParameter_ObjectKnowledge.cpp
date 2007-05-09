// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_ObjectKnowledge.h"
#include "Network_Def.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_ObjectKnowledge constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_ObjectKnowledge::MissionParameter_ObjectKnowledge( const ASN1T_MissionParameter& asn )
    : MissionParameter_ABC( asn )
    , objectKnowledge_    ( asn.value.u.knowledgeObject )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_ObjectKnowledge destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_ObjectKnowledge::~MissionParameter_ObjectKnowledge()
{
    // NOTHING
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MissionParameter_ObjectKnowledge::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_ObjectKnowledge::Send( ASN1T_MissionParameter& asn ) const
{
    asn.null_value              = bNullValue_;
    asn.value.t                 = T_MissionParameter_value_knowledgeObject;
    asn.value.u.knowledgeObject = objectKnowledge_;
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_ObjectKnowledge::AsnDelete
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_ObjectKnowledge::AsnDelete( ASN1T_MissionParameter& /*asn*/ ) const
{
    // NOTHING
}

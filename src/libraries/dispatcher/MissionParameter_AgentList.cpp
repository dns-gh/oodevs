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
    for( unsigned i = 0; i != asn.value.u.unitList->n; ++i )
        agents_.push_back( asn.value.u.unitList->elem[i] );
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
    asn.value.t                    = T_MissionParameter_value_unitList;
    asn.value.u.unitList = new ASN1T_UnitList();
    SendContainerValues< ASN1T_UnitList, ASN1T_Unit, T_OIDVector >( agents_, *asn.value.u.unitList ); 
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_AgentList::AsnDelete
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_AgentList::AsnDelete( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.unitList->n > 0 )
        delete [] asn.value.u.unitList->elem;
    delete asn.value.u.unitList;
}

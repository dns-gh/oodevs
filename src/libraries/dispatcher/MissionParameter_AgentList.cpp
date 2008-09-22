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
#include "game_asn/Simulation.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_AgentList constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_AgentList::MissionParameter_AgentList( const ASN1T_MissionParameter& asn )
    : MissionParameter_ABC( asn )
{
    std::copy( asn.value.u.unitList->elem, asn.value.u.unitList->elem + asn.value.u.unitList->n, std::back_inserter( agents_ ) );
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_AgentList destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_AgentList::~MissionParameter_AgentList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_AgentList::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_AgentList::Send( ASN1T_MissionParameter& asn ) const
{
    asn.null_value = bNullValue_;
    asn.value.t = T_MissionParameter_value_unitList;
    asn.value.u.unitList = new ASN1T_UnitList();
    asn.value.u.unitList->n = agents_.size();
    asn.value.u.unitList->elem = agents_.empty() ? 0 : (int*)&agents_.front();
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_AgentList::AsnDelete
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_AgentList::AsnDelete( ASN1T_MissionParameter& asn ) const
{
    delete asn.value.u.unitList;
}

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_ObjectiveList.h"
#include "Network_Def.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_ObjectiveList constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_ObjectiveList::MissionParameter_ObjectiveList( const ASN1T_MissionParameter& asn )
    : MissionParameter_ABC( asn )
{
    objects_.reserve( asn.value.u.missionObjectiveList->n );
    for( unsigned i = 0; i != asn.value.u.missionObjectiveList->n; ++i )
        objects_.push_back( Objective( asn.value.u.missionObjectiveList->elem[i] ) );
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_ObjectiveList destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_ObjectiveList::~MissionParameter_ObjectiveList()
{
    // NOTHING
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MissionParameter_ObjectiveList::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_ObjectiveList::Send( ASN1T_MissionParameter& asn ) const
{
    asn.null_value                   = bNullValue_;
    asn.value.t                      = T_MissionParameter_value_missionObjectiveList;
    asn.value.u.missionObjectiveList = new ASN1T_MissionObjectiveList();

    asn.value.u.missionObjectiveList->n = objects_.size();
    if( !objects_.empty() )
    {
        asn.value.u.missionObjectiveList->elem = new ASN1T_MissionObjective[ objects_.size() ];
        unsigned int i = 0;
        for( T_ObjectiveVector::const_iterator it = objects_.begin(); it != objects_.end(); ++it, ++i )
            (*it).Send( asn.value.u.missionObjectiveList->elem[ i ] );
    }
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_ObjectiveList::AsnDelete
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_ObjectiveList::AsnDelete( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.missionObjectiveList->n > 0 )
        delete [] asn.value.u.missionObjectiveList->elem;
    delete asn.value.u.missionObjectiveList;
}

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_GenObjectList.h"
#include "Network_Def.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_GenObjectList constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_GenObjectList::MissionParameter_GenObjectList( const ASN1T_MissionParameter& asn )
    : MissionParameter_ABC( asn )
{
    objects_.reserve( asn.value.u.plannedWorkList->n );
    for( unsigned i = 0; i != asn.value.u.plannedWorkList->n; ++i )
        objects_.push_back( GenObject( asn.value.u.plannedWorkList->elem[i] ) );
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_GenObjectList destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_GenObjectList::~MissionParameter_GenObjectList()
{
    // NOTHING
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MissionParameter_GenObjectList::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_GenObjectList::Send( ASN1T_MissionParameter& asn ) const
{
    asn.null_value                   = bNullValue_;
    asn.value.t                      = T_MissionParameter_value_plannedWorkList;
    asn.value.u.plannedWorkList = new ASN1T_PlannedWorkList();

    asn.value.u.plannedWorkList->n = objects_.size();
    if( !objects_.empty() )
    {
        asn.value.u.plannedWorkList->elem = new ASN1T_PlannedWork[ objects_.size() ];
        unsigned int i = 0;
        for( T_GenObjectVector::const_iterator it = objects_.begin(); it != objects_.end(); ++it, ++i )
            (*it).Send( asn.value.u.plannedWorkList->elem[ i ] );
    }
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_GenObjectList::AsnDelete
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_GenObjectList::AsnDelete( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.plannedWorkList->n > 0 )
        delete [] asn.value.u.plannedWorkList->elem;
    delete asn.value.u.plannedWorkList;
}

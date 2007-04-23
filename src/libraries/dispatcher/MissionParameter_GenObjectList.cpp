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
    objects_.reserve( asn.value.u.listMissionGenObject->n );
    for( unsigned i = 0; i != asn.value.u.listMissionGenObject->n; ++i )
        objects_.push_back( GenObject( asn.value.u.listMissionGenObject->elem[i] ) );
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
    asn.value.t                      = T_MissionParameter_value_listMissionGenObject;
    asn.value.u.listMissionGenObject = new ASN1T_ListMissionGenObject();

    asn.value.u.listMissionGenObject->n = objects_.size();
    if( !objects_.empty() )
    {
        asn.value.u.listMissionGenObject->elem = new ASN1T_MissionGenObject[ objects_.size() ];
        uint i = 0;
        for( T_GenObjectVector::const_iterator it = objects_.begin(); it != objects_.end(); ++it, ++i )
            (*it).Send( asn.value.u.listMissionGenObject->elem[ i ] );
    }
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_GenObjectList::AsnDelete
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_GenObjectList::AsnDelete( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.listMissionGenObject->n > 0 )
    {
        for( unsigned i = 0; i != asn.value.u.listMissionGenObject->n; ++i )
            GenObject::AsnDelete( asn.value.u.listMissionGenObject->elem[i] );
        delete [] asn.value.u.listMissionGenObject->elem;
    }
    delete asn.value.u.listMissionGenObject;
}

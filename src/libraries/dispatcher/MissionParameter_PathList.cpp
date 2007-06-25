// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_PathList.h"
#include "Network_Def.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_PathList constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_PathList::MissionParameter_PathList( const ASN1T_MissionParameter& asn )
    : MissionParameter_ABC( asn )
{
    paths_.reserve( asn.value.u.pathList->n );
    for( unsigned i = 0; i != asn.value.u.pathList->n; ++i )
        paths_.push_back( Localisation( asn.value.u.pathList->elem[i] ) );
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_PathList destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_PathList::~MissionParameter_PathList()
{
    // NOTHING
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MissionParameter_PathList::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_PathList::Send( ASN1T_MissionParameter& asn ) const
{
    asn.null_value             = bNullValue_;
    asn.value.t                = T_MissionParameter_value_pathList;
    asn.value.u.pathList = new ASN1T_PathList();

    asn.value.u.pathList->n = paths_.size();
    if( !paths_.empty() )
    {
        asn.value.u.pathList->elem = new ASN1T_Path[ paths_.size() ];
        unsigned int i = 0;
        for( T_LocalisationVector::const_iterator it = paths_.begin(); it != paths_.end(); ++it, ++i )
            (*it).Send( asn.value.u.pathList->elem[ i ] );
    }
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_PathList::AsnDelete
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_PathList::AsnDelete( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.pathList->n > 0 )
    {
        for( unsigned i = 0; i != asn.value.u.pathList->n; ++i )
            Localisation::AsnDelete( asn.value.u.pathList->elem[i] );
        delete [] asn.value.u.pathList->elem;
    }
    delete asn.value.u.pathList;
}

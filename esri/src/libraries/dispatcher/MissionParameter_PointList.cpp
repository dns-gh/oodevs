// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_PointList.h"
#include "Network_Def.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_PointList constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_PointList::MissionParameter_PointList( const ASN1T_MissionParameter& asn )
    : MissionParameter_ABC( asn )
{
    points_.reserve( asn.value.u.listPoint->n );
    for( unsigned i = 0; i != asn.value.u.listPoint->n; ++i )
        points_.push_back( Localisation( asn.value.u.listPoint->elem[i] ) );
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_PointList destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_PointList::~MissionParameter_PointList()
{
    // NOTHING
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MissionParameter_PointList::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_PointList::Send( ASN1T_MissionParameter& asn ) const
{
    asn.null_value        = bNullValue_;
    asn.value.t           = T_MissionParameter_value_listPoint;
    asn.value.u.listPoint = new ASN1T_ListPoint();

    asn.value.u.listPoint->n = points_.size();
    if( !points_.empty() )
    {
        asn.value.u.listPoint->elem = new ASN1T_Point[ points_.size() ];
        uint i = 0;
        for( T_LocalisationVector::const_iterator it = points_.begin(); it != points_.end(); ++it, ++i )
            (*it).Send( asn.value.u.listPoint->elem[ i ] );
    }
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_PointList::AsnDelete
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_PointList::AsnDelete( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.listPoint->n > 0 )
    {
        for( unsigned i = 0; i != asn.value.u.listPoint->n; ++i )
            Localisation::AsnDelete( asn.value.u.listPoint->elem[i] );
        delete [] asn.value.u.listPoint->elem;
    }
    delete asn.value.u.listPoint;
}

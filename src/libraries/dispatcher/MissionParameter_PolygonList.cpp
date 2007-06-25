// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_PolygonList.h"
#include "Network_Def.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_PolygonList constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_PolygonList::MissionParameter_PolygonList( const ASN1T_MissionParameter& asn )
    : MissionParameter_ABC( asn )
{
    polygons_.reserve( asn.value.u.polygonList->n );
    for( unsigned i = 0; i != asn.value.u.polygonList->n; ++i )
        polygons_.push_back( Localisation( asn.value.u.polygonList->elem[i] ) );
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_PolygonList destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_PolygonList::~MissionParameter_PolygonList()
{
    // NOTHING
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MissionParameter_PolygonList::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_PolygonList::Send( ASN1T_MissionParameter& asn ) const
{
    asn.null_value          = bNullValue_;
    asn.value.t             = T_MissionParameter_value_polygonList;
    asn.value.u.polygonList = new ASN1T_PolygonList();

    asn.value.u.polygonList->n = polygons_.size();
    if( !polygons_.empty() )
    {
        asn.value.u.polygonList->elem = new ASN1T_Polygon[ polygons_.size() ];
        unsigned int i = 0;
        for( T_LocalisationVector::const_iterator it = polygons_.begin(); it != polygons_.end(); ++it, ++i )
            (*it).Send( asn.value.u.polygonList->elem[ i ] );
    }
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_PolygonList::AsnDelete
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_PolygonList::AsnDelete( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.polygonList->n > 0 )
    {
        for( unsigned i = 0; i != asn.value.u.polygonList->n; ++i )
            Localisation::AsnDelete( asn.value.u.polygonList->elem[i] );
        delete [] asn.value.u.polygonList->elem;
    }
    delete asn.value.u.polygonList;
}

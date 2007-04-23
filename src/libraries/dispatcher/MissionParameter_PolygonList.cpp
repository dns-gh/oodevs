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
    polygons_.reserve( asn.value.u.listPolygon->n );
    for( unsigned i = 0; i != asn.value.u.listPolygon->n; ++i )
        polygons_.push_back( Localisation( asn.value.u.listPolygon->elem[i] ) );
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
    asn.value.t             = T_MissionParameter_value_listPolygon;
    asn.value.u.listPolygon = new ASN1T_ListPolygon();

    asn.value.u.listPolygon->n = polygons_.size();
    if( !polygons_.empty() )
    {
        asn.value.u.listPolygon->elem = new ASN1T_Polygon[ polygons_.size() ];
        uint i = 0;
        for( T_LocalisationVector::const_iterator it = polygons_.begin(); it != polygons_.end(); ++it, ++i )
            (*it).Send( asn.value.u.listPolygon->elem[ i ] );
    }
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_PolygonList::AsnDelete
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_PolygonList::AsnDelete( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.listPolygon->n > 0 )
    {
        for( unsigned i = 0; i != asn.value.u.listPolygon->n; ++i )
            Localisation::AsnDelete( asn.value.u.listPolygon->elem[i] );
        delete [] asn.value.u.listPolygon->elem;
    }
    delete asn.value.u.listPolygon;
}

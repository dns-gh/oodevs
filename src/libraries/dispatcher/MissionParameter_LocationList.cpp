// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_LocationList.h"
#include "Network_Def.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_LocationList constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_LocationList::MissionParameter_LocationList( const ASN1T_MissionParameter& asn )
    : MissionParameter_ABC( asn )
{
    locations_.reserve( asn.value.u.listLocalisation->n );
    for( unsigned i = 0; i != asn.value.u.listLocalisation->n; ++i )
        locations_.push_back( Localisation( asn.value.u.listLocalisation->elem[i] ) );
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_LocationList destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
MissionParameter_LocationList::~MissionParameter_LocationList()
{
    // NOTHING
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MissionParameter_LocationList::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_LocationList::Send( ASN1T_MissionParameter& asn ) const
{
    asn.null_value               = bNullValue_;
    asn.value.t                  = T_MissionParameter_value_listLocalisation;
    asn.value.u.listLocalisation = new ASN1T_ListLocalisation();

    asn.value.u.listLocalisation->n = locations_.size();
    if( !locations_.empty() )
    {
        asn.value.u.listLocalisation->elem = new ASN1T_Localisation[ locations_.size() ];
        uint i = 0;
        for( T_LocalisationVector::const_iterator it = locations_.begin(); it != locations_.end(); ++it, ++i )
            (*it).Send( asn.value.u.listLocalisation->elem[ i ] );
    }
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_LocationList::AsnDelete
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void MissionParameter_LocationList::AsnDelete( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.listLocalisation->n > 0 )
    {
        for( unsigned i = 0; i != asn.value.u.listLocalisation->n; ++i )
            Localisation::AsnDelete( asn.value.u.listLocalisation->elem[i] );
        delete [] asn.value.u.listLocalisation->elem;
    }
    delete asn.value.u.listLocalisation;
}

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_IntelligenceList.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_IntelligenceList constructor
// Created: SBO 2008-03-04
// -----------------------------------------------------------------------------
MissionParameter_IntelligenceList::MissionParameter_IntelligenceList( const ASN1T_MissionParameter& asn )
    : MissionParameter_ABC( asn )
{
    for( unsigned int i = 0; i < asn.value.u.intelligenceList->n; ++i )
        intelligenceOrders_.push_back( IntelligenceOrder( asn.value.u.intelligenceList->elem[ i ] ) );
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_IntelligenceList destructor
// Created: SBO 2008-03-04
// -----------------------------------------------------------------------------
MissionParameter_IntelligenceList::~MissionParameter_IntelligenceList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_IntelligenceList::Send
// Created: SBO 2008-03-04
// -----------------------------------------------------------------------------
void MissionParameter_IntelligenceList::Send( ASN1T_MissionParameter& asn ) const
{
    asn.null_value = bNullValue_;
    asn.value.t = T_MissionParameter_value_intelligenceList;
    asn.value.u.intelligenceList = new ASN1T_IntelligenceList();
    asn.value.u.intelligenceList->n = intelligenceOrders_.size();
    asn.value.u.intelligenceList->elem = intelligenceOrders_.empty() ? 0 : new ASN1T_Intelligence[ asn.value.u.intelligenceList->n ];
    unsigned int i = 0;
    for( std::vector< IntelligenceOrder >::const_iterator it = intelligenceOrders_.begin(); it != intelligenceOrders_.end(); ++it, ++i )
        it->Send( asn.value.u.intelligenceList->elem[i] );
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_IntelligenceList::AsnDelete
// Created: SBO 2008-03-04
// -----------------------------------------------------------------------------
void MissionParameter_IntelligenceList::AsnDelete( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.intelligenceList->n > 0 )
        delete[] asn.value.u.intelligenceList->elem;
    delete asn.value.u.intelligenceList;
}

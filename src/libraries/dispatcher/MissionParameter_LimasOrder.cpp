// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "MissionParameter_LimasOrder.h"
#include "LimaOrder.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: MissionParameter_LimasOrder constructor
// Created: SBO 2008-03-04
// -----------------------------------------------------------------------------
MissionParameter_LimasOrder::MissionParameter_LimasOrder( Model& model, const ASN1T_MissionParameter& asn )
    : MissionParameter_ABC( asn )
{
    for( unsigned int i = 0; i < asn.value.u.limasOrder->n; ++i )
        limaOrders_.Create( model, i, asn.value.u.limasOrder->elem[ i ] );
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_LimasOrder destructor
// Created: SBO 2008-03-04
// -----------------------------------------------------------------------------
MissionParameter_LimasOrder::~MissionParameter_LimasOrder()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_LimasOrder::Send
// Created: SBO 2008-03-04
// -----------------------------------------------------------------------------
void MissionParameter_LimasOrder::Send( ASN1T_MissionParameter& asn ) const
{
    asn.null_value = bNullValue_;
    asn.value.t    = T_MissionParameter_value_limasOrder;
    asn.value.u.limasOrder = new ASN1T_LimasOrder();
    limaOrders_.Send< ASN1T_LimasOrder, ASN1T_LimaOrder >( *asn.value.u.limasOrder );
}

// -----------------------------------------------------------------------------
// Name: MissionParameter_LimasOrder::AsnDelete
// Created: SBO 2008-03-04
// -----------------------------------------------------------------------------
void MissionParameter_LimasOrder::AsnDelete( ASN1T_MissionParameter& asn ) const
{
    if( asn.value.u.limasOrder->n > 0 )
    {
        for( unsigned i = 0; i < asn.value.u.limasOrder->n; ++i )
            LimaOrder::AsnDelete( asn.value.u.limasOrder->elem[i] );
        delete[] asn.value.u.limasOrder->elem;
    }
    delete asn.value.u.limasOrder;
}

// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "OrderContext.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: OrderContext constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
OrderContext::OrderContext( Model& model, const ASN1T_OrderContext& asn )
    : dangerDirection_   ( asn.direction_dangereuse )
    , limaOrders_        ()
    , rightLimit_        ()
    , leftLimit_         ()
    , bRightLimitPresent_( false ) 
    , bLeftLimitPresent_ ( false ) 
{
    if( asn.m.limite_droitePresent )
    {
        bRightLimitPresent_ = true;
        rightLimit_.Update( asn.limite_droite );
    }
    if( asn.m.limite_gauchePresent )
    {
        bLeftLimitPresent_ = true;
        leftLimit_.Update( asn.limite_gauche );
    }
    for( unsigned int i = 0; i < asn.limas.n; ++i )
        limaOrders_.Create( model, i, asn.limas.elem[ i ] );
}

// -----------------------------------------------------------------------------
// Name: OrderContext constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
OrderContext::OrderContext()
    : dangerDirection_   ( 0 )
    , limaOrders_        ()
    , rightLimit_        ()
    , leftLimit_         ()
    , bRightLimitPresent_( false )
    , bLeftLimitPresent_ ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: OrderContext destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
OrderContext::~OrderContext()
{
    // NOTHING
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: OrderContext::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void OrderContext::Send( ASN1T_OrderContext& asn ) const
{
    if( bRightLimitPresent_ )
    {
        rightLimit_.Send( asn.limite_droite );
        asn.m.limite_droitePresent = 1;
    }
    if( bLeftLimitPresent_ )
    {
        leftLimit_.Send( asn.limite_gauche );
        asn.m.limite_gauchePresent = 1;
    }
    limaOrders_.Send< ASN1T_LimasOrder, ASN1T_LimaOrder >( asn.limas );
    asn.direction_dangereuse = dangerDirection_;
}
    
// -----------------------------------------------------------------------------
// Name: OrderContext::AsnDelete
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void OrderContext::AsnDelete( ASN1T_OrderContext& asn )
{
    if( asn.m.limite_droitePresent )
        Localisation::AsnDelete( asn.limite_droite );
    if( asn.m.limite_gauchePresent )
        Localisation::AsnDelete( asn.limite_gauche );
    if( asn.limas.n > 0 )
    {
        for( unsigned i = 0; i != asn.limas.n; ++i )
            LimaOrder::AsnDelete( asn.limas.elem[i] );
        delete [] asn.limas.elem;
    }
}

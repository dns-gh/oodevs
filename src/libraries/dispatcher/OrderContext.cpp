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
#include "LimaOrder.h"
#include "IntelligenceOrder.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: OrderContext constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
OrderContext::OrderContext()
    : dangerDirection_   ( 0 )
    , limaOrders_        ()
    , intelligenceOrders_()
    , rightLimit_        ()
    , leftLimit_         ()
    , bRightLimitPresent_( false )
    , bLeftLimitPresent_ ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: OrderContext constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
OrderContext::OrderContext( Model& model, const ASN1T_OrderContext& asn )
    : dangerDirection_   ( asn.direction_dangereuse )
    , limaOrders_        ()
    , intelligenceOrders_()
    , rightLimit_        ()
    , leftLimit_         ()
    , bRightLimitPresent_( asn.m.limite_droitePresent ? true : false ) 
    , bLeftLimitPresent_ ( asn.m.limite_gauchePresent ? true : false )
{
    if( bRightLimitPresent_ )
        rightLimit_.Update( asn.limite_droite );
    if( bLeftLimitPresent_ )
        leftLimit_.Update( asn.limite_gauche );
    for( unsigned int i = 0; i < asn.limas.n; ++i )
        limaOrders_.Create( model, i, asn.limas.elem[ i ] );
    for( unsigned int i = 0; i < asn.intelligences.n; ++i )
        intelligenceOrders_.Create( model, i, asn.intelligences.elem[ i ] );
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
    asn.m.limite_droitePresent = bRightLimitPresent_;
    asn.m.limite_gauchePresent = bLeftLimitPresent_;
    if( bRightLimitPresent_ )
        rightLimit_.Send( asn.limite_droite );
    if( bLeftLimitPresent_ )
        leftLimit_.Send( asn.limite_gauche );
    limaOrders_.Send< ASN1T_LimasOrder, ASN1T_LimaOrder >( asn.limas );
    intelligenceOrders_.Send< ASN1T_IntelligenceList, ASN1T_Intelligence >( asn.intelligences );
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
        for( unsigned i = 0; i < asn.limas.n; ++i )
            LimaOrder::AsnDelete( asn.limas.elem[i] );
        delete[] asn.limas.elem;
    }
    if( asn.intelligences.n > 0 )
        delete[] asn.intelligences.elem;
}

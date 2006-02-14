// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "SupplyStates.h"
#include "Controller.h"

// -----------------------------------------------------------------------------
// Name: SupplyStates constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
SupplyStates::SupplyStates( Controller& controller )
    : controller_( controller )
{

}

// -----------------------------------------------------------------------------
// Name: SupplyStates destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
SupplyStates::~SupplyStates()
{

}

// -----------------------------------------------------------------------------
// Name: SupplyStates::Update
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void SupplyStates::Update( const ASN1T_MsgLogRavitaillementEtat& message )
{
    if( message.m.chaine_activeePresent )
        bChainEnabled_ = message.chaine_activee;

    if( message.m.disponibilites_transporteurs_convoisPresent )
    {
        dispoTransporters_.resize( message.disponibilites_transporteurs_convois.n );
        for( uint i = 0; i < message.disponibilites_transporteurs_convois.n; ++i )
            dispoTransporters_[i] = std::make_pair( message.disponibilites_transporteurs_convois.elem[i].type_equipement, message.disponibilites_transporteurs_convois.elem[i].pourcentage_disponibilite );
    }
    if( message.m.disponibilites_chefs_convoisPresent )
    {
        dispoCommanders_.resize( message.disponibilites_chefs_convois.n );
        for( uint i = 0; i < message.disponibilites_chefs_convois.n; ++i )
            dispoCommanders_[i] = std::make_pair( message.disponibilites_chefs_convois.elem[i].type_equipement, message.disponibilites_chefs_convois.elem[i].pourcentage_disponibilite );
    }

    if( message.m.stocksPresent )
    {
        uint nSize = message.stocks.n;
        while( nSize > 0 )
        {
            ASN1T_DotationStock& value = message.stocks.elem[ --nSize ];
            stocks_[ value.ressource_id ] = value.quantite_disponible;
        }
    }
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: SupplyStates::Update
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void SupplyStates::Update( const ASN1T_MsgLogRavitaillementQuotas& message )
{
    quotas_.resize( message.quotas.n );
    for( uint i = 0; i < message.quotas.n; ++i )
        quotas_[ i ] = std::make_pair( message.quotas.elem[i].ressource_id, message.quotas.elem[i].quota_disponible );
    controller_.Update( *this );
}

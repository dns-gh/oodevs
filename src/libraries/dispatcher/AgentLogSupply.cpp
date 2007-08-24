// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "AgentLogSupply.h"

#include "Model.h"
#include "Automat.h"
#include "Agent.h"
#include "Network_Def.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: AgentLogSupply constructor
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
AgentLogSupply::AgentLogSupply( Model& model, const Agent& agent, const ASN1T_MsgLogSupplyState& asnMsg )
    : agent_                ( agent )
    , model_                ( model )
    , bSystemEnabled_       ( false )
    , stocks_               ()
    , convoyersAvailability_()
{
    Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: AgentLogSupply destructor
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
AgentLogSupply::~AgentLogSupply()
{

}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: AgentLogSupply::Update
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void AgentLogSupply::Update( const ASN1T_MsgLogSupplyState& asnMsg )
{
    if( asnMsg.m.chaine_activeePresent )
        bSystemEnabled_ = asnMsg.chaine_activee;

    if( asnMsg.m.disponibilites_transporteurs_convoisPresent )
    {
        convoyersAvailability_.Clear();
        for( unsigned int i = 0; i < asnMsg.disponibilites_transporteurs_convois.n; ++i )
            convoyersAvailability_.Create( model_, i, asnMsg.disponibilites_transporteurs_convois.elem[ i ] );
    }

    if( asnMsg.m.stocksPresent )
    {
        for( unsigned i = 0; i < asnMsg.stocks.n; ++i )
        {
            Dotation* pDotation = stocks_.Find( asnMsg.stocks.elem[ i ].ressource_id );
            if( pDotation )
                pDotation->Update( asnMsg.stocks.elem[ i ] );
            else
                pDotation = &stocks_.Create( model_, asnMsg.stocks.elem[ i ].ressource_id, asnMsg.stocks.elem[ i ] );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: AgentLogSupply::Send
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void AgentLogSupply::Send( ClientPublisher_ABC& publisher ) const
{
    AsnMsgSimToClientLogSupplyState asn;

    asn().oid_pion = agent_.GetID();

    asn().m.chaine_activeePresent                       = 1;
    asn().m.disponibilites_transporteurs_convoisPresent = 1;
    asn().m.stocksPresent                               = 1;

    asn().chaine_activee = bSystemEnabled_;

    convoyersAvailability_.Send< ASN1T__SeqOfLogSupplyEquimentAvailability, ASN1T_LogSupplyEquimentAvailability >( asn().disponibilites_transporteurs_convois );

    stocks_.Send< ASN1T__SeqOfDotationStock, ASN1T_DotationStock >( asn().stocks );

    asn.Send( publisher );

    if( asn().disponibilites_transporteurs_convois.n > 0 )
        delete [] asn().disponibilites_transporteurs_convois.elem;
    if( asn().stocks.n > 0 )
        delete [] asn().stocks.elem;

}

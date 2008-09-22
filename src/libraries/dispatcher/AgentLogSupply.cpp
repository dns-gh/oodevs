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
#include "Dotation.h"
#include "ClientPublisher_ABC.h"
#include "clients_kernel/Agent_ABC.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: AgentLogSupply constructor
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
AgentLogSupply::AgentLogSupply( const kernel::Agent_ABC& agent, const ASN1T_MsgLogSupplyState& asnMsg )
    : agent_                ( agent )
    , bSystemEnabled_       ( false )
{
    Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: AgentLogSupply destructor
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
AgentLogSupply::~AgentLogSupply()
{
    stocks_.DeleteAll();
}

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
        convoyersAvailability_.clear();
        for( unsigned int i = 0; i < asnMsg.disponibilites_transporteurs_convois.n; ++i )
            convoyersAvailability_.push_back( T_Availability( asnMsg.disponibilites_transporteurs_convois.elem[ i ] ) );
    }

    if( asnMsg.m.stocksPresent )
        for( unsigned int i = 0; i < asnMsg.stocks.n; ++i )
        {
            Dotation* pDotation = stocks_.Find( asnMsg.stocks.elem[ i ].ressource_id );
            if( pDotation )
                pDotation->Update( asnMsg.stocks.elem[ i ] );
            else
            {
                pDotation = new Dotation( asnMsg.stocks.elem[ i ] );
                stocks_.Register( asnMsg.stocks.elem[ i ].ressource_id, *pDotation );
            }
        }
}

// -----------------------------------------------------------------------------
// Name: AgentLogSupply::Send
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void AgentLogSupply::Send( ClientPublisher_ABC& publisher ) const
{
    client::LogSupplyState asn;

    asn().oid_pion = agent_.GetId();

    asn().m.chaine_activeePresent                       = 1;
    asn().m.disponibilites_transporteurs_convoisPresent = 1;
    asn().m.stocksPresent                               = 1;

    asn().chaine_activee = bSystemEnabled_;

    {
        asn().disponibilites_transporteurs_convois.n = convoyersAvailability_.size();
        asn().disponibilites_transporteurs_convois.elem = asn().disponibilites_transporteurs_convois.n > 0 ? new ASN1T_LogSupplyEquimentAvailability[ asn().disponibilites_transporteurs_convois.n ] : 0;
        unsigned int i = 0;
        for( std::vector< T_Availability >::const_iterator it = convoyersAvailability_.begin(); it != convoyersAvailability_.end(); ++it )
            it->Send( asn().disponibilites_transporteurs_convois.elem[i++] );
    }
    {
        asn().stocks.n = stocks_.Count();
        asn().stocks.elem = asn().stocks.n > 0 ? new ASN1T_DotationStock[ asn().stocks.n ] : 0;
        unsigned int i = 0;
        for( kernel::Iterator< const Dotation& > it = stocks_.CreateIterator(); it.HasMoreElements(); )
            it.NextElement().Send( asn().stocks.elem[i++] );
    }
    asn.Send( publisher );

    if( asn().disponibilites_transporteurs_convois.n > 0 )
        delete [] asn().disponibilites_transporteurs_convois.elem;
    if( asn().stocks.n > 0 )
        delete [] asn().stocks.elem;
}

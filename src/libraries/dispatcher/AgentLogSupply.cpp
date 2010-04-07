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
#include "protocol/ClientPublisher_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "protocol/clientsenders.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: AgentLogSupply constructor
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
AgentLogSupply::AgentLogSupply( const kernel::Agent_ABC& agent, const MsgsSimToClient::MsgLogSupplyState& asnMsg )
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
void AgentLogSupply::Update( const MsgsSimToClient::MsgLogSupplyState& asnMsg )
{
    if( asnMsg.has_chaine_activee()  )
        bSystemEnabled_ = asnMsg.chaine_activee() != 0;

    if( asnMsg.has_disponibilites_transporteurs_convois()  )
    {
        convoyersAvailability_.clear();
        for( int i = 0; i < asnMsg.disponibilites_transporteurs_convois().elem_size(); ++i )
            convoyersAvailability_.push_back( T_Availability( asnMsg.disponibilites_transporteurs_convois().elem( i ) ) );
    }

    if( asnMsg.has_stocks()  )
        for( int i = 0; i < asnMsg.stocks().elem_size(); ++i )
        {
            Dotation* pDotation = stocks_.Find( asnMsg.stocks().elem( i ).ressource_id() );
            if( pDotation )
                pDotation->Update( asnMsg.stocks().elem( i ) );
            else
            {
                pDotation = new Dotation( asnMsg.stocks().elem( i ) );
                stocks_.Register( asnMsg.stocks().elem( i ).ressource_id(), *pDotation );
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

    asn().set_oid_pion( agent_.GetId() );

//    asn().set_chaine_activeePresent( 1 );
//    asn().set_disponibilites_transporteurs_convoisPresent( 1 );
//    asn().set_stocksPresent( 1 );

    asn().set_chaine_activee( bSystemEnabled_ );

    {
        for( std::vector< T_Availability >::const_iterator it = convoyersAvailability_.begin(); it != convoyersAvailability_.end(); ++it )
            it->Send( *asn().mutable_disponibilites_transporteurs_convois()->add_elem() );
    }
    {
        for( tools::Iterator< const Dotation& > it = stocks_.CreateIterator(); it.HasMoreElements(); )
            it.NextElement().Send( *asn().mutable_stocks()->add_elem() );
    }
    asn.Send( publisher );

    if( asn().disponibilites_transporteurs_convois().elem_size() > 0 )
        asn().mutable_disponibilites_transporteurs_convois()->Clear();
    if( asn().stocks().elem_size() > 0 )
        asn().mutable_stocks()->Clear();
}

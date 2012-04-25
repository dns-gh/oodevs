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
#include "protocol/ClientSenders.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: AgentLogSupply constructor
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
AgentLogSupply::AgentLogSupply( const kernel::Agent_ABC& agent, const sword::LogSupplyState& asnMsg )
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
void AgentLogSupply::Update( const sword::LogSupplyState& asnMsg )
{
    if( asnMsg.has_chain()  )
        bSystemEnabled_ = asnMsg.chain() != 0;

    if( asnMsg.has_transporters()  )
    {
        convoyersAvailability_.clear();
        for( int i = 0; i < asnMsg.transporters().elem_size(); ++i )
            convoyersAvailability_.push_back( T_Availability( asnMsg.transporters().elem( i ) ) );
    }

    if( asnMsg.has_stocks() )
        for( int i = 0; i < asnMsg.stocks().elem_size(); ++i )
        {
            Dotation* pDotation = stocks_.Find( asnMsg.stocks().elem( i ).resource().id() );
            if( pDotation )
                pDotation->Update( asnMsg.stocks().elem( i ) );
            else
            {
                pDotation = new Dotation( asnMsg.stocks().elem( i ) );
                stocks_.Register( asnMsg.stocks().elem( i ).resource().id(), *pDotation );
            }
        }

    if( asnMsg.has_network_stock() )
    {
        int id = asnMsg.network_stock().resource().id();
        if( id == 0 )
            resourceNetworkStock_.reset();
        else
            resourceNetworkStock_.reset( new Dotation( asnMsg.network_stock() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentLogSupply::Send
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
void AgentLogSupply::Send( ClientPublisher_ABC& publisher ) const
{
    client::LogSupplyState asn;
    asn().mutable_unit()->set_id( agent_.GetId() );
    asn().set_chain( bSystemEnabled_ );
    {
        for( std::vector< T_Availability >::const_iterator it = convoyersAvailability_.begin(); it != convoyersAvailability_.end(); ++it )
            it->Send( *asn().mutable_transporters()->add_elem() );
    }
    {
        for( tools::Iterator< const Dotation& > it = stocks_.CreateIterator(); it.HasMoreElements(); )
            it.NextElement().Send( *asn().mutable_stocks()->add_elem() );
    }
    if( resourceNetworkStock_.get() )
        resourceNetworkStock_->Send( *asn().mutable_network_stock() );
    asn.Send( publisher );
}

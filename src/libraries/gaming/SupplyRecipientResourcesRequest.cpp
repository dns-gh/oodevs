// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "SupplyRecipientResourcesRequest.h"
#include "SupplyResourceRequest.h"
#include "protocol/Protocol.h"
#include "clients_kernel/Automat_ABC.h"
#include <boost/foreach.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: SupplyRecipientResourcesRequest constructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
SupplyRecipientResourcesRequest::SupplyRecipientResourcesRequest( const tools::Resolver_ABC< DotationType >& dotationResolver, const tools::Resolver_ABC< kernel::Automat_ABC >& resolver, const sword::SupplyRecipientResourcesRequest& msg )
    : recipient_       ( resolver.Get( msg.recipient().id() ) )
    , dotationResolver_( dotationResolver )
{
    BOOST_FOREACH( const sword::SupplyResourceRequest& data, msg.resources() )
    {
        Register( data.resource().id(), *new SupplyResourceRequest( dotationResolver_.Get( data.resource().id() ),
            data.requested(), data.granted(), data.convoyed() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: SupplyRecipientResourcesRequest destructor
// Created: AGE 2006-02-28
// -----------------------------------------------------------------------------
SupplyRecipientResourcesRequest::~SupplyRecipientResourcesRequest()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SupplyRecipientResourcesRequest::Update
// Created: LGY 2014-01-27
// -----------------------------------------------------------------------------
void SupplyRecipientResourcesRequest::Update( const sword::SupplyRecipientResourceRequests& msg )
{
    bool delivered = true;
    BOOST_FOREACH( const sword::SupplyRecipientResourcesRequest& data, msg.requests() )
    {
        if( data.recipient().id() == recipient_.GetId() )
        {
            // Update supply resource
            Apply( boost::bind( &SupplyResourceRequest::Update, _1,  boost::cref( data.resources() ) ) );
            // If one resource is present, the resident isn't totally supply
            delivered = false;
        }
    }
    // if the message doesns't contain resources for the recipient,
    //    then all resources are delivered and the resident is
    //    totally supply
    if( delivered )
        Apply( boost::bind( &SupplyResourceRequest::Deliver, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: SupplyRecipientResourcesRequest::GetRecipientTooltip
// Created: MMC 2012-10-29
// -----------------------------------------------------------------------------
QString SupplyRecipientResourcesRequest::GetRecipientTooltip() const
{
    return recipient_.GetTooltip();
}

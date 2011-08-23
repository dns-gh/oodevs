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
    Update( msg );
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
// Name: LogSupplyConsign::Update
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void SupplyRecipientResourcesRequest::Update( const sword::SupplyRecipientResourcesRequest& msg )
{
    BOOST_FOREACH( const sword::SupplyResourceRequest& data, msg.resources() )
    {
        SupplyResourceRequest* request = Find( data.resource().id() );
        if( request )
        {
            request->requested_ = data.requested();
            request->granted_   = data.granted();
            request->convoyed_  = data.convoyed();
        }
        else
        {
            Register( data.resource().id(), *new SupplyResourceRequest( dotationResolver_.Get( data.resource().id() ), 
                                                                   data.requested(),
                                                                   data.granted(),
                                                                   data.convoyed() ) );
        }
    }
}

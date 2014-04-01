// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "LogSupplyRecipientResourcesRequest.h"
#include "LogSupplyResourceRequest.h"
#include "Automat.h"
#include "Model.h"
#include "protocol/Protocol.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: LogSupplyRecipientResourcesRequest constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
LogSupplyRecipientResourcesRequest::LogSupplyRecipientResourcesRequest( const Model& model, const sword::SupplyRecipientResourcesRequest& msg )
    : model_  ( model )
    , automat_( model.Automats().Get( msg.recipient().id() ) )
{
    Update( msg );
}

// -----------------------------------------------------------------------------
// Name: LogSupplyRecipientResourcesRequest destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
LogSupplyRecipientResourcesRequest::~LogSupplyRecipientResourcesRequest()
{
    resources_.DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: LogSupplyRecipientResourcesRequest::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void LogSupplyRecipientResourcesRequest::Update( const sword::SupplyRecipientResourcesRequest& msg )
{
    const auto& resources = msg.resources();
    for( auto it = resources.begin(); it != resources.end(); ++it )
    {
        LogSupplyResourceRequest* resource = resources_.Find( it->resource().id() );
        if( resource )
            resource->Update( *it );
        else
        {
            resource = new LogSupplyResourceRequest( model_, *it );
            resources_.Register( it->resource().id(), *resource );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: LogSupplyRecipientResourcesRequest::Send
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void LogSupplyRecipientResourcesRequest::Send( sword::SupplyRecipientResourcesRequest& msg ) const
{
    msg.mutable_recipient()->set_id( automat_.GetId() );
    for( tools::Iterator< const LogSupplyResourceRequest& > it = resources_.CreateIterator(); it.HasMoreElements(); )
        it.NextElement().Send( *msg.add_resources() );
}

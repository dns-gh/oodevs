// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "LogSupplyResourceRequest.h"
#include "protocol/Protocol.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: LogSupplyResourceRequest constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
LogSupplyResourceRequest::LogSupplyResourceRequest( const Model& /*model*/, const sword::SupplyResourceRequest& msg )
    : nDotationType_( msg.resource().id() )
    , nNbrRequested_( msg.requested() )
    , nNbrGranted_  ( msg.granted() )
    , nNbrConvoyed_ ( msg.convoyed() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogSupplyResourceRequest destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
LogSupplyResourceRequest::~LogSupplyResourceRequest()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogSupplyResourceRequest::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void LogSupplyResourceRequest::Update( const sword::SupplyResourceRequest& msg )
{
    nNbrRequested_ = msg.requested();
    nNbrGranted_ = msg.granted();
    nNbrConvoyed_ = msg.convoyed();
}

// -----------------------------------------------------------------------------
// Name: LogSupplyResourceRequest::Send
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void LogSupplyResourceRequest::Send( sword::SupplyResourceRequest& msg ) const
{
    msg.mutable_resource()->set_id( nDotationType_ );
    msg.set_requested( nNbrRequested_ );
    msg.set_granted( nNbrGranted_ );
    msg.set_convoyed( nNbrConvoyed_ );
}

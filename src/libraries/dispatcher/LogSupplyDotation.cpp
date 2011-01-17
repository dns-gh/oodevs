// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "LogSupplyDotation.h"
#include "protocol/Protocol.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: LogSupplyDotation constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
LogSupplyDotation::LogSupplyDotation( const Model& /*model*/, const sword::DotationQuery& msg )
    : nDotationType_( msg.resource().id() )
    , nNbrRequested_( msg.requested() )
    , nNbrGranted_  ( msg.granted() )
    , nNbrConvoyed_ ( msg.convoyed() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogSupplyDotation destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
LogSupplyDotation::~LogSupplyDotation()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogSupplyDotation::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void LogSupplyDotation::Update( const sword::DotationQuery& msg )
{
    nNbrRequested_ = msg.requested();
    nNbrGranted_ = msg.granted();
    nNbrConvoyed_ = msg.convoyed();
}

// -----------------------------------------------------------------------------
// Name: LogSupplyDotation::Send
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void LogSupplyDotation::Send( sword::DotationQuery& msg ) const
{
    msg.mutable_resource()->set_id( nDotationType_ );
    msg.set_requested( nNbrRequested_ );
    msg.set_granted( nNbrGranted_ );
    msg.set_convoyed( nNbrConvoyed_ );
}

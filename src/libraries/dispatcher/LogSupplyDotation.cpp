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
#include "protocol/protocol.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: LogSupplyDotation constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
LogSupplyDotation::LogSupplyDotation( const Model& /*model*/, const MsgsSimToClient::MsgDotationQuery& asn )
    : nDotationType_( asn.resource().id()       )
    , nNbrRequested_( asn.quantite_demandee()   )
    , nNbrGranted_  ( asn.quantite_accordee()   )
    , nNbrConvoyed_ ( asn.quantite_en_transit() )
{
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
void LogSupplyDotation::Update( const MsgsSimToClient::MsgDotationQuery& asn )
{
    nNbrRequested_ = asn.quantite_demandee();
    nNbrGranted_   = asn.quantite_accordee();
    nNbrConvoyed_  = asn.quantite_en_transit();
}

// -----------------------------------------------------------------------------
// Name: LogSupplyDotation::Send
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void LogSupplyDotation::Send( MsgsSimToClient::MsgDotationQuery& asn ) const
{
    asn.mutable_resource()->set_id( nDotationType_ );
    asn.set_quantite_demandee( nNbrRequested_ );
    asn.set_quantite_accordee( nNbrGranted_ );
    asn.set_quantite_en_transit( nNbrConvoyed_ );
}

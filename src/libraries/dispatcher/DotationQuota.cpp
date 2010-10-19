// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "DotationQuota.h"
#include "protocol/SimulationSenders.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: DotationQuota constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
DotationQuota::DotationQuota( const Common::MsgDotationQuota& message )
   : nDotationType_( message.ressource_id().id() )
   , nQuota_       ( message.quota_disponible() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DotationQuota destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
DotationQuota::~DotationQuota()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DotationQuota::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void DotationQuota::Update( const Common::MsgDotationQuota& message )
{
    nQuota_ = message.quota_disponible();
}

// -----------------------------------------------------------------------------
// Name: DotationQuota::Send
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void DotationQuota::Send( Common::MsgDotationQuota& message ) const
{
    message.mutable_ressource_id()->set_id( nDotationType_ );
    message.set_quota_disponible( nQuota_ );
}

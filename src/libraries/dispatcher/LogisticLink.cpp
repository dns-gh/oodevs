// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "LogisticLink.h"
#include "DotationQuota.h"
#include "LogisticEntity_ABC.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/ClientSenders.h"
#include <boost/bind.hpp>

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: LogisticLink constructor
// Created: NLD 2011-03-11
// -----------------------------------------------------------------------------
LogisticLink::LogisticLink( const LogisticHierarchyOwner_ABC& owner, const LogisticEntity_ABC& superior )
    : owner_   ( owner )
    , superior_( superior )
    , updated_ ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticLink destructor
// Created: AHC 2010-10-11
// -----------------------------------------------------------------------------
LogisticLink::~LogisticLink()
{
    quotas_.DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: LogisticLink::Update
// Created: NLD 2011-01-17
// -----------------------------------------------------------------------------
void LogisticLink::Update( const sword::SeqOfDotationQuota& msg )
{
    updated_ = true;
    quotas_.DeleteAll();
    for( int i = 0; i < msg.elem_size(); i++ )
    {
        DotationQuota* quota = new DotationQuota( msg.elem( i ) );
        quotas_.Register( quota->GetDotationType(), *quota );
    }
}

namespace
{
    void SerializeQuota( ::sword::SeqOfDotationQuota& message, const DotationQuota& quota )
    {
        quota.Send( *message.add_elem() );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticLink::SendFullUpdate
// Created: NLD 2011-01-17
// -----------------------------------------------------------------------------
void LogisticLink::SendFullUpdate( ClientPublisher_ABC& publisher ) const
{
    if( updated_ )
    {
        client::LogSupplyQuotas msg;
        owner_.Send( *msg().mutable_supplied() );
        superior_.Send( *msg().mutable_supplier() );
        msg().mutable_quotas();
        quotas_.Apply( boost::bind( &SerializeQuota, boost::ref( *msg().mutable_quotas() ), _1 ) );
        msg.Send( publisher );
    }
}

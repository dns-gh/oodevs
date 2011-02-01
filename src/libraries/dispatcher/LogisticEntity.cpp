// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "LogisticEntity.h"
#include "Automat_ABC.h"
#include "DotationQuota.h"
#include "Formation_ABC.h"
#include "Model_ABC.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/ClientSenders.h"
#include "tools/Resolver_ABC.h"
#include <boost/bind.hpp>

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: LogisticEntity constructor
// Created: AHC 2010-10-11
// -----------------------------------------------------------------------------
LogisticEntity::LogisticEntity( const tools::Resolver_ABC< Formation_ABC >& formations, const tools::Resolver_ABC< Automat_ABC >& automats, const kernel::LogisticLevel& level )
    : formations_( formations )
    , automats_  ( automats )
    , pLogisticLevel_( &level )
    , pSuperiorFormation_( 0 )
    , pSuperieurAutomat_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticEntity destructor
// Created: AHC 2010-10-11
// -----------------------------------------------------------------------------
LogisticEntity::~LogisticEntity()
{
    quotas_.DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: LogisticEntity::DoUpdate
// Created: AHC 2010-10-11
// -----------------------------------------------------------------------------
void LogisticEntity::DoUpdate( const sword::ChangeLogisticLinks& msg )
{
    // TODO Handle error
    if( msg.logistic_base().has_automat() )
        pSuperieurAutomat_ = automats_.Find( msg.logistic_base().automat().id() );
    if( msg.logistic_base().has_formation() )
        pSuperiorFormation_ = formations_.Find( msg.logistic_base().formation().id() );
}

// -----------------------------------------------------------------------------
// Name: LogisticEntity::Fill
// Created: AHC 2010-10-11
// -----------------------------------------------------------------------------
void LogisticEntity::Fill( client::ChangeLogisticLinks& msg ) const
{
    if( pSuperieurAutomat_ )
        msg().mutable_logistic_base()->mutable_automat()->set_id( pSuperieurAutomat_->GetId() );
    else if( pSuperiorFormation_ )
        msg().mutable_logistic_base()->mutable_formation()->set_id( pSuperiorFormation_->GetId() );
}

// -----------------------------------------------------------------------------
// Name: LogisticEntity::FiGetLogisticLevelll
// Created: AHC 2010-10-11
// -----------------------------------------------------------------------------
const kernel::LogisticLevel& LogisticEntity::GetLogisticLevel() const
{
    return *pLogisticLevel_;
}

// -----------------------------------------------------------------------------
// Name: LogisticEntity::DoUpdate
// Created: NLD 2007-03-29
// -----------------------------------------------------------------------------
void LogisticEntity::DoUpdate( const sword::LogSupplyQuotas& msg )
{
    quotas_.DeleteAll();
    for( int i = 0; i < msg.quotas().elem_size(); ++i )
    {
        DotationQuota* quota = new DotationQuota( msg.quotas().elem( i ) );
        quotas_.Register( msg.quotas().elem( i ).resource().id(), *quota );
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
// Name: LogisticEntity::FiGetLogisticLevelll
// Created: AHC 2010-10-11
// -----------------------------------------------------------------------------
void LogisticEntity::Fill( client::LogSupplyQuotas& msg ) const
{
    quotas_.Apply( boost::bind( &SerializeQuota, boost::ref( *msg().mutable_quotas() ), _1 ) );
}

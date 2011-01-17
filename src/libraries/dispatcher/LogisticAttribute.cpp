// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "LogisticAttribute.h"
#include "Model_ABC.h"
#include "Automat_ABC.h"
#include "protocol/Protocol.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: LogisticAttribute constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
LogisticAttribute::LogisticAttribute( const Model_ABC& model, const sword::ObjectAttributes& asnMsg )
    : automats_( model.Automats() )
    , pTC2_( 0 )
{
    Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: LogisticAttribute destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
LogisticAttribute::~LogisticAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticAttribute::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void LogisticAttribute::Update( const sword::ObjectAttributes& asnMsg )
{
    if ( asnMsg.has_logistic()  )
        pTC2_ = &automats_.Get( asnMsg.logistic().combat_train().id() );    
}

// -----------------------------------------------------------------------------
// Name: LogisticAttribute::Send
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void LogisticAttribute::Send( sword::ObjectAttributes& asnMsg ) const
{
    if( !pTC2_ )
        throw std::runtime_error( __FUNCTION__ ": logistic superior is not defined" );
    asnMsg.mutable_logistic()->mutable_combat_train()->set_id( pTC2_->GetId() );
}

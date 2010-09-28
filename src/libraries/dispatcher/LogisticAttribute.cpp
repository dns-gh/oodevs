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
#include "protocol/protocol.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: LogisticAttribute constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
LogisticAttribute::LogisticAttribute( const Model_ABC& model, const Common::ObjectAttributes& asnMsg )
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
void LogisticAttribute::Update( const Common::ObjectAttributes& asnMsg )
{
    if ( asnMsg.has_logistic()  )
        pTC2_ = &automats_.Get( asnMsg.logistic().tc2().id() );    
}

// -----------------------------------------------------------------------------
// Name: LogisticAttribute::Send
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void LogisticAttribute::Send( Common::ObjectAttributes& asnMsg ) const
{
    if( !pTC2_ )
        throw std::runtime_error( __FUNCTION__ ": logistic superior is not defined" );
    asnMsg.mutable_logistic()->mutable_tc2()->set_id( pTC2_->GetId() );
}

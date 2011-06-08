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
#include "Formation_ABC.h"
#include "LogisticEntity_ABC.h"
#include "protocol/Protocol.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: LogisticAttribute constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
LogisticAttribute::LogisticAttribute( const Model_ABC& model, const sword::ObjectAttributes& asnMsg )
    : automats_( model.Automats() )
    , formations_( model.Formations() )
    , pLogisticBase_( 0 )
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
void LogisticAttribute::Update( const sword::ObjectAttributes& msg )
{
    if ( msg.has_logistic() && msg.logistic().has_logistic_superior() )
    {
        if( msg.logistic().logistic_superior().has_automat() )
        {
            Automat_ABC* tmp = automats_.Find( msg.logistic().logistic_superior().automat().id() );
            if( tmp )
                pLogisticBase_ = tmp->GetLogisticEntity();
        }
        else if( msg.logistic().logistic_superior().has_formation() )
        {
            Formation_ABC* tmp = formations_.Find( msg.logistic().logistic_superior().formation().id() );
            if( tmp )
                pLogisticBase_ = tmp->GetLogisticEntity();
        }
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticAttribute::Send
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void LogisticAttribute::Send( sword::ObjectAttributes& msg ) const
{
    if( pLogisticBase_ )
        pLogisticBase_->Send( *msg.mutable_logistic()->mutable_logistic_superior() );
    else
        msg.mutable_logistic();
}
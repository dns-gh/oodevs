// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "NBCTypeAttribute.h"
#include "protocol/SimulationSenders.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: NBCTypeAttribute constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
NBCTypeAttribute::NBCTypeAttribute( const sword::ObjectAttributes& asnMsg )
    : agent_( 0 )
    , concentration_( 0 )
    , sourceLifeDuration_( 0 )
{
    Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: NBCTypeAttribute destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
NBCTypeAttribute::~NBCTypeAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NBCTypeAttribute::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void NBCTypeAttribute::Update( const sword::ObjectAttributes& asnMsg )
{
    if( !asnMsg.has_nbc_agent()  )
        return;
    agent_ = asnMsg.nbc_agent().agent().id();
    concentration_ = asnMsg.nbc_agent().concentration();
    sourceLifeDuration_ = asnMsg.nbc_agent().source_life_duration();
}

// -----------------------------------------------------------------------------
// Name: NBCTypeAttribute::Send
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void NBCTypeAttribute::Send( sword::ObjectAttributes& asnMsg ) const
{
    asnMsg.mutable_nbc_agent()->mutable_agent()->set_id( agent_ );
    asnMsg.mutable_nbc_agent()->set_concentration( concentration_ );
    asnMsg.mutable_nbc_agent()->set_source_life_duration( sourceLifeDuration_ );
}


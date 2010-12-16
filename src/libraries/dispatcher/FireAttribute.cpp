// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "FireAttribute.h"
#include "protocol/SimulationSenders.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: FireAttribute constructor
// Created: RFT 2006-09-26
// -----------------------------------------------------------------------------
FireAttribute::FireAttribute( const sword::ObjectAttributes& asnMsg )
    : fireClass_( asnMsg.fire().class_name() )
    , maxCombustionEnergy_( asnMsg.fire().max_combustion_energy() )
        
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FireAttribute destructor
// Created: RFT 2006-09-26
// -----------------------------------------------------------------------------
FireAttribute::~FireAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FireAttribute::Update
// Created: RFT 2006-09-26
// -----------------------------------------------------------------------------
void FireAttribute::Update( const sword::ObjectAttributes& asnMsg )
{
    if( asnMsg.has_fire() )
    {
        fireClass_ = asnMsg.fire().class_name();
        maxCombustionEnergy_ = asnMsg.fire().max_combustion_energy();
    }
}

// -----------------------------------------------------------------------------
// Name: FireAttribute::Send
// Created: RFT 2006-09-27
// -----------------------------------------------------------------------------
void FireAttribute::Send( sword::ObjectAttributes& asnMsg ) const
{
    asnMsg.mutable_fire()->set_class_name( fireClass_ );
    asnMsg.mutable_fire()->set_max_combustion_energy( maxCombustionEnergy_ );
}

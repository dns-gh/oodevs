// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "BurnAttribute.h"
#include "protocol/SimulationSenders.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: BurnAttribute constructor
// Created: RFT 2006-09-26
// -----------------------------------------------------------------------------
BurnAttribute::BurnAttribute( const sword::ObjectAttributes& asnMsg )
: currentHeat_( asnMsg.burn().current_heat() )
, combustionEnergy_( asnMsg.burn().combustion_energy() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BurnAttribute destructor
// Created: RFT 2006-09-26
// -----------------------------------------------------------------------------
BurnAttribute::~BurnAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BurnAttribute::Update
// Created: RFT 2006-09-26
// -----------------------------------------------------------------------------
void BurnAttribute::Update( const sword::ObjectAttributes& asnMsg )
{
    if( asnMsg.has_burn() )
    {
        currentHeat_ = asnMsg.burn().current_heat();
        combustionEnergy_ = asnMsg.burn().combustion_energy();
    }
}

// -----------------------------------------------------------------------------
// Name: BurnAttribute::Send
// Created: RFT 2006-09-27
// -----------------------------------------------------------------------------
void BurnAttribute::Send( sword::ObjectAttributes& asnMsg ) const
{
    asnMsg.mutable_burn()->set_current_heat( currentHeat_ );
    asnMsg.mutable_burn()->set_combustion_energy( combustionEnergy_ );
}
